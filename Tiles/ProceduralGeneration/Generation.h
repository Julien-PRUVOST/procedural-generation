#pragma once
#include <map>
#include <memory>
#include <random>
#include <utility>
#include <vector>

// Todo : remove those includes
#include "Math.h"
#include "Rule.h"
#include "../Tile.h"
#include "../Hexagonal/Grid.h"


namespace ProceduralGeneration
{
	using std::vector;

	using Grid = Hexagonal::Grid;

	class GenerationProcess
	{
	public:
		using rule_t = Rule;
		using pattern_t = Pattern;
		using rule_ptr = std::unique_ptr<Rule>;
		using pattern_ptr = std::shared_ptr<pattern_t>;

		using tag_type = pattern_t::tag_type;

	private:
		vector<rule_t> rules {};
		std::map<tag_type, vector<pattern_t>> tiles {};

		/**
		 * \brief The reproductibility of the seed cannot be guaranteed if multiple generation commands are used simultaneously on the same GenerationProcess
		 */
		std::mt19937 prng;

	public:
		void add(rule_t &&rule)
		{
			rules.push_back(std::move(rule));
		}

		void add(pattern_t &&tile)
		{
			tiles[tile.tag].push_back(std::move(tile));
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
		 Grid::tile_type& getRandomPos(Grid &grid)
		{
			std::uniform_int_distribution<typename Grid::size_type> posChooser{ 0, grid.getSize() };

			return grid.getTile(posChooser(prng));
		}

		pattern_t & getPattern(tag_type &tag)
		{
			const std::uniform_int_distribution<size_t> tileChooser{ 0, tiles[tag].size() };

			return tiles[tag][tileChooser(prng)];
		}

		void setPatternAt(pattern_t &pattern, Grid::tile_type &pos)
		{
			pos.setCenter(pattern.center);
			pos.setRing(pattern.externalRing);
		}

#pragma region Path
	private:
		auto getAngles(const typename Grid::tile_type &referenceTile, const vector<typename Grid::tile_ptr> &neighbors)
		{
			vector<float> angles;

			for (const auto& neighbor : neighbors)
			{
				angles.push_back(Grid::tile_type::getAngleDegrees(referenceTile.getTileAngleTo(*neighbor)));
			}

			return angles;
		}

		auto getRelativeAngles(float referenceAngle, const vector<float> &angles)
		{
			vector<float> relativeAngles;

			for (float angle: angles)
			{
				relativeAngles.push_back(Math::getAngularDifference(angle, referenceAngle, 360.0f));
			}

			return relativeAngles;
		}

		void filterNeighbors(vector<typename Grid::tile_ptr> &neighbors, vector<float>& angles, vector<float>& relativeAngles, float threshold = 360.0f)
		{
			vector<size_t> toErase;
			for (size_t i = 0; i != neighbors.size(); ++i)
			{
				if (std::abs(relativeAngles[i]) > threshold) toErase.push_back(i);
			}

			for(auto p = toErase.rbegin(); p != toErase.rend(); ++p)
			{
				auto it_Angles = angles.begin();
				std::advance(it_Angles, *p);
				angles.erase(it_Angles);

				auto it_relativeAngles = relativeAngles.begin();
				std::advance(it_relativeAngles, *p);
				relativeAngles.erase(it_relativeAngles);

				auto it_neighbors = neighbors.begin();
				std::advance(it_neighbors, *p);
				neighbors.erase(it_neighbors);
			}
			
		}

		auto getTileProbability(vector<typename Grid::tile_ptr> &pool, const vector<float> &relativeAngles)
		{
			vector<float> probability;

			float range = 0.0f;

			for(size_t i = 0; i != relativeAngles.size(); ++i)
			{
				for (size_t j = i; j != relativeAngles.size(); ++j)
				{
					range = std::max(range, std::abs(Math::getAngularDifference(relativeAngles[i], relativeAngles[j])));
				}
			}

			for (size_t i = 0; i != relativeAngles.size(); ++i)
			{
				probability.push_back((range - std::abs(relativeAngles[i])) / (range * (static_cast<float>(relativeAngles.size()) - 1)));
			}

			return probability;
		}

		typename Grid::tile_type& getNextTileInPath(Grid &grid, typename Grid::tile_type &current, const typename Grid::tile_type &goal)
		{
			vector<typename Grid::tile_ptr> neighbors =  grid.getNeighbors(current);

			// Todo : Wierd to have two angles at 180°
			vector<float> angles = getAngles(current, neighbors);

			const float referenceAngle = current.getAngleDegrees(goal);
			vector<float> relativeAngles = getRelativeAngles(referenceAngle, angles);

			filterNeighbors(neighbors, angles, relativeAngles, 90.0f);

			vector<float> probability = getTileProbability(neighbors, relativeAngles);

			std::discrete_distribution<size_t> distribution(probability.begin(), probability.end());

			return *neighbors[distribution(prng)];
		}
#pragma endregion

	public:
		void buildPath(
			Grid& grid,
			tag_type tagStart,
			tag_type tagGoal,
			tag_type tagPath,
			typename Grid::tile_type &start,
			typename Grid::tile_type &goal)
		{
			pattern_t &pattern = getPattern(tagStart);
			setPatternAt(pattern, start);

			typename Grid::tile_type current = start;

			while (current != goal)
			{
				typename Grid::tile_type next = getNextTileInPath(grid, current, goal);

			//	next.setRing()
			//	// Chose the next tile
			//	// Place the constraint for the next tile
			//	// Fill the current tile with a pattern available
			//	// Current = next

			//	pattern_ptr pattern = getPatternAt(tagStart);


			//	current = next;
			}
			
		}

		void buildPath(Grid& grid, tag_type tagStart, tag_type tagGoal,	tag_type tagPath)
		{
			buildPath(grid, std::move(tagStart), std::move(tagGoal), std::move(tagPath), getRandomPos(grid), getRandomPos(grid));
		}

		void buildPath(Grid& grid, tag_type tagStart, tag_type tagGoal, typename Grid::tile_type& start, typename Grid::tile_type& goal, pattern_t::element_type element)
		{
			pattern_t &pattern = getPattern(tagStart);
			setPatternAt(pattern, start);

			typename Grid::tile_type* current = &start;

			while (*current != goal)
			{
				typename Grid::tile_type* next = &getNextTileInPath(grid, *current, goal);

				current->setCenter(element);

				int angleToNext = current->getTileAngleTo(*next);

				// Should be a ring of variable size depending on the grid
				vector<pattern_t::element_type> ring{ element, {}, {}, {}, {}, {} };

				ring = Pattern::rotateRing(ring, angleToNext);

				current->addToRing(ring);

				next->setCenter(element);
				next->addToRing(Pattern::rotateRing(ring, 3));

				current = next;
			}
		}

	};
}
