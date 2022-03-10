#pragma once
#include <map>
#include <memory>
#include <random>
#include <utility>
#include <vector>
#include <functional>

// Todo : remove those includes
#include "../ShinMathLib/Math.h"
#include "../ShinMathLib/VectorMath.h"
#include "../Tile.h"
#include "../Hexagonal/Grid.h"


namespace ProceduralGen
{
	using std::vector;
	using namespace shinmathlib;	

	using Grid = Hexagonal::Grid;

	template <class Pattern = Pattern<Element<char>>>
	class GenerationProcess
	{
	public:
		using pattern_t = Pattern;
		using pattern_ptr = std::shared_ptr<pattern_t>;

		using tag_type = typename pattern_t::tag_t;

		using seed_type = int;

		static bool trueCondition(const Grid::tile_ptr&) { return true; }
		static bool falseCondition(const Grid::tile_ptr&) { return false; }

	private:
		std::map<tag_type, vector<pattern_t>> tiles {};

		/**
		 * \brief The reproductibility of the seed cannot be guaranteed if multiple generation commands are used simultaneously on the same GenerationProcess
		 */
		std::mt19937 prng;
		seed_type seed {};

		std::mt19937 prngSeed;

	public:
		void add(pattern_t &&tile)
		{
			tiles[tile.tag].push_back(std::move(tile));
		}

	public:
		void setRandomSeed()
		{
			std::uniform_int_distribution<seed_type> seedChooser{};
			setSeed(seedChooser(prngSeed));
		}

		void setSeed(seed_type newSeed)
		{
			seed = newSeed;
			prng.seed(seed);
		}

		seed_type getSeed() const
		{
			return seed;
		}

	public:
		// template <class Grid>
		void populate(Grid &grid)
		{
			
		}

	public:
		/**
		 * \brief Is public only for debug puposes
		 * \return A random position on the grid
		 */
		Grid::tile_ptr getRandomPos(Grid &grid)
		{
			const size_t index = VectorMath::chooseIndex(grid.getSize(), prng);
			return grid.getTile(index);
		}

		Grid::tile_ptr getRandomValidPos(Grid& grid,
			const std::function<bool(const Grid::tile_ptr&)>& condition = trueCondition)
		{
			const vector<Grid::tile_ptr> validTiles = grid.getValidTiles();
			vector<Grid::tile_ptr> pool{};
			for (const auto & tile : validTiles)
			{
				if (condition(tile))
					pool.push_back(tile);
			}
			const size_t index = VectorMath::chooseIndex(pool.size(), prng);
			return pool[index];
		}

		vector<pattern_t>& getPatterns(const tag_type& tag)
		{
			return tiles[tag];
		}

		static void eraseGrid(Grid &grid)
		 {
			 for (auto tile : grid.grid)
			 {
				 tile->erase();
			 }
		 }

#pragma region Path
	private:
		/// Unused
		static vector<float> getAngles(const Grid::tile_ptr& referenceTile, const vector<Grid::tile_ptr>& neighbors)
		{
			vector<float> angles;

			for (const auto& neighbor : neighbors)
			{
				angles.push_back(referenceTile->getAngleDegreesTo(*neighbor));
			}

			return angles;
		}

		static vector<float> getRelativeAngles(const Grid::tile_ptr &referenceTile, float referenceAngle, const vector<Grid::tile_ptr> &neighbors)
		{
			vector<float> angles;

			for (const auto& neighbor : neighbors)
			{
				const float angle = referenceTile->getAngleDegreesTo(*neighbor);
				angles.push_back(Math::angularDifference(angle, referenceAngle, 360.0f));
			}

			return angles;
		}

		static void filterNeighbors(vector<Grid::tile_ptr> &neighbors, vector<float>& relativeAngles, float threshold = 360.0f)
		{
			const vector<size_t> indices = VectorMath::filteredIndex(relativeAngles, [&threshold](const float& angle) { return std::abs(angle) > threshold; });

			VectorMath::eraseByIndex(relativeAngles, indices);
			VectorMath::eraseByIndex(neighbors, indices);			
		}

		static vector<float> getTileProbability(const vector<float> &relativeAngles)
		{
			vector<float> probability;

			float range = 0.0f;

			for(size_t i = 0; i != relativeAngles.size(); ++i)
			{
				for (size_t j = i; j != relativeAngles.size(); ++j)
				{
					range = std::max(range, std::abs(Math::angularDifference(relativeAngles[i], relativeAngles[j])));
				}
			}

			for (size_t i = 0; i != relativeAngles.size(); ++i)
			{
				probability.push_back(range - std::abs(relativeAngles[i]));
			}

			return probability;
		}

		Grid::tile_ptr getNextTileInPath(Grid &grid, const Grid::tile_ptr &current, const Grid::tile_ptr &goal, const vector<Grid::tile_ptr> &visited)
		{
			if (current->distance(*goal) == 1) return goal;

			vector<Grid::tile_ptr> neighbors =  grid.getValidNeighbors(*current);

			VectorMath::eraseOncePtr(neighbors, visited);

			const float referenceAngle = current->getAngleDegreesTo(*goal);
			vector<float> relativeAngles = getRelativeAngles(current, referenceAngle, neighbors);

			filterNeighbors(neighbors, relativeAngles, 90.0f);

			const vector<float> probability = getTileProbability(relativeAngles);

			return *VectorMath::choose(neighbors, probability, prng);
		}

		static vector<pattern_t> getCompatiblePatterns(const Grid::tile_ptr& current, const vector<pattern_t>& patterns, vector<size_t>& outTileAngles)
		{
			vector<pattern_t> compatiblePatterns;

			for (size_t i = 0; i != patterns.size(); ++i)
			{
				size_t angle;
				if (current->compatible(patterns[i], angle))
				{
					compatiblePatterns.push_back(patterns[i]);
					outTileAngles.push_back(angle);
				}
			}

			return compatiblePatterns;
		}

		class pattern_not_found final : public std::exception {};

		pattern_t choosePattern(const Grid::tile_ptr& current, const vector<pattern_t>& patterns, size_t &outAngle)
		{
			vector<size_t> tileAngles;

			// Todo : I want compatible pattern on a specific order : the contraints placed must be respected, while the one added are just a bonus
			const vector<pattern_t> compatiblePatterns = getCompatiblePatterns(current, patterns, tileAngles);

			if (!compatiblePatterns.empty())
			{
				vector<size_t> probability;
				for (const auto& pattern : compatiblePatterns)
				{
					probability.push_back(pattern.weight);
				}

				const size_t index = VectorMath::chooseIndex(probability, prng);
				outAngle = tileAngles[index];
				return compatiblePatterns[index];
			}

			throw pattern_not_found{};
		}

		static const tag_type& getTag(const Grid::tile_ptr& current, const Grid::tile_ptr& start, const Grid::tile_ptr& goal,
		                              const tag_type &tagPath, const tag_type& tagStart, const tag_type& tagGoal)
		{
			if (*current == *start) return tagStart;
			if (*current == *goal) return tagGoal;
			return tagPath;
		}

		void getAndApplyPattern(Grid::tile_ptr& current, const Grid::tile_ptr& start, const Grid::tile_ptr& goal,
			const tag_type& tagPath, const tag_type& tagStart, const tag_type& tagGoal)
		{
			size_t angle;
			const tag_type& tag = getTag(current, start, goal, tagPath, tagStart, tagGoal);
			const pattern_t pattern = choosePattern(current, getPatterns(tag), angle);

			current->mergePattern(pattern, angle);
		}

		class interrupted : public std::exception {};

		void buildPathIteration(Grid& grid, Grid::tile_ptr& current, const Grid::tile_ptr& start, const Grid::tile_ptr& goal,
			const tag_type& tagPath, const tag_type& tagStart, const tag_type& tagGoal, const Grid::tile_type::pattern_t::element_t &linkingElement,
			vector<Grid::tile_ptr>& visited,
			const std::function<bool(const Grid::tile_ptr&)> &stopCondition)
		{
			visited.push_back(current);

			// Get the next tile
			// Put the path element on the constraints of current, in direction of next. Same for next to current
			// Choose a pattern to put on current
			// Put that pattern on current
			// current = next
			Grid::tile_ptr next;
			try
			{
				next = getNextTileInPath(grid, current, goal, visited);
				current->setContraintTo(*next, linkingElement);
				next->setContraintTo(*current, linkingElement);
			}
			catch (VectorMath::cannot_choose_in_empty_range)
			{
				current = goal;
				next = goal;
				throw interrupted{};
			}

			getAndApplyPattern(current, start, goal, tagPath, tagStart, tagGoal);

			current = next;

			if (stopCondition(current)) throw interrupted{};
		}

	public:

		void buildPath(Grid& grid, tag_type tagStart, tag_type tagGoal, tag_type tagPath, Grid::tile_ptr start, Grid::tile_ptr goal, Grid::tile_type::pattern_t::element_t linkingElement,
			std::function<bool(const Grid::tile_ptr&)> stopCondition = falseCondition)
		{
			vector<Grid::tile_ptr> visited;

			Grid::tile_ptr current = start;

			while (*current != *goal)
			{
				try
				{
					buildPathIteration(grid, current, start, goal, tagPath, tagStart, tagGoal, linkingElement, visited, stopCondition);
				}
				catch (interrupted)
				{
					return;
				}
			}
			getAndApplyPattern(current, start, goal, tagPath, tagStart, tagGoal);
		}

		void buildPath(Grid& grid, tag_type tagStart, tag_type tagGoal, tag_type tagPath, Grid::tile_type::pattern_t::element_t linkingElement,
			Grid::tile_ptr start = nullptr, Grid::tile_ptr goal = nullptr,
			std::function<bool(const Grid::tile_ptr&)> startCondition = trueCondition,
			std::function<bool(const Grid::tile_ptr&)> stopCondition = falseCondition,
			std::function<bool(const Grid::tile_ptr&)> goalCondition = trueCondition)
		{
			if (!start) start = getRandomValidPos(grid, startCondition);
			if (!goal) goal = getRandomValidPos(grid, goalCondition);
			buildPath(grid, std::move(tagStart), std::move(tagGoal), std::move(tagPath), start, goal, linkingElement, std::move(stopCondition));
		}
#pragma endregion

#pragma region River
	private:
		Grid::tile_ptr getNextTileInRiver(Grid& grid, const Grid::tile_ptr& current, const vector<Grid::tile_ptr>& visited)
		{
			vector<Grid::tile_ptr> neighbors = grid.getValidNeighbors(*current);

			VectorMath::eraseOncePtr(neighbors, visited);

			return *VectorMath::choose(neighbors, prng);
		}

		pattern_t getAndApplyPattern(Grid::tile_ptr& current, const tag_type& tag)
		{
			size_t angle;
			const pattern_t pattern = choosePattern(current, getPatterns(tag), angle);
			current->mergePattern(pattern, angle);

			return pattern;
		}

		void buildRiverIteration(Grid& grid, Grid::tile_ptr& current, const tag_type& tag, const Grid::tile_type::pattern_t::element_t& linkingElement,
			size_t& currentRiverSize, vector<Grid::tile_ptr>& visited)
		{
			visited.push_back(current);

			// Get the next tile
			// Put the path element on the constraints of current, in direction of next. Same for next to current
			// Choose a pattern to put on current
			// Put that pattern on current
			// current = next

			Grid::tile_ptr next; 

			try
			{
				next = getNextTileInRiver(grid, current, visited);

				current->setContraintTo(*next, linkingElement);
				next->setContraintTo(*current, linkingElement);
			}
			catch (VectorMath::cannot_choose_in_empty_range)
			{
				throw interrupted{};
			}

			const pattern_t pattern = getAndApplyPattern(current, tag);

			currentRiverSize += static_cast<size_t>(!pattern.center.isDefault());
			currentRiverSize += VectorMath::count_if(pattern.externalRing, [](const Grid::tile_type::pattern_t::element_t& element) { return !element.isDefault(); });

			current = next;
		}

	public:
		void buildRiver(Grid& grid, tag_type tagStart, tag_type tagEnd, tag_type tagRiver, Grid::tile_ptr start, Grid::tile_type::pattern_t::element_t linkingElement, size_t riverMinLength)
		{
			vector<Grid::tile_ptr> visited;

			size_t currentRiverSize = 0;

			Grid::tile_ptr current = start;

			while (currentRiverSize < riverMinLength)
			{
				tag_type& tag = (*current == *start) ? tagStart : tagRiver;

				try
				{
					buildRiverIteration(grid, current, tag, linkingElement, currentRiverSize, visited);
				}
				catch (interrupted)
				{
					break;
				}
			}

			getAndApplyPattern(current, tagEnd);
		}

		void buildRiver(Grid& grid, tag_type tagStart, tag_type tagGoal, tag_type tagRiver, Grid::tile_type::pattern_t::element_t linkingElement, size_t riverMinLength)
		{
			Grid::tile_ptr start = getRandomValidPos(grid);
			buildRiver(grid, std::move(tagStart), std::move(tagGoal), std::move(tagRiver), start, linkingElement, riverMinLength);
		}

#pragma endregion

	};
}
