#pragma once
#include <map>
#include <memory>
#include <random>
#include <utility>
#include <vector>

// Todo : remove those includes
#include "Math.h"
#include "Rule.h"
#include "Tile.h"
#include "../Hexagonal/Grid.h"

namespace ProceduralGeneration
{
	using namespace std;
	using Grid = Hexagonal::Grid;

	class GenerationProcess
	{
	public:
		using rule_t = Rule;
		using pattern_t = Pattern;
		using rule_ptr = unique_ptr<Rule>;
		using pattern_ptr = shared_ptr<pattern_t>;

		using tag_type = typename pattern_t::tag_type;

	private:
		vector<rule_ptr> rules {};
		map<tag_type, vector<pattern_ptr>> tiles {};

		/**
		 * \brief The reproductibility of the seed cannot be guaranteed if multiple generation commands are used simultaneously on the same GenerationProcess
		 */
		mt19937 prng;

	public:
		void add(rule_ptr &&rule)
		{
			rules.push_back(std::move(rule));
		}

		void add(pattern_ptr&&tile)
		{
			tiles[tile->tag].push_back(std::move(tile));
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
			uniform_int_distribution<typename Grid::size_type> posChooser{ 0, grid.getSize() };

			return grid.getTile(posChooser(prng));
		}

		pattern_ptr getPattern(tag_type &tag)
		{
			const uniform_int_distribution<size_t> tileChooser{ 0, tiles[tag].size() };

			return tiles[tag][tileChooser(prng)];
		}

		void setPatternAt(pattern_ptr pattern, Grid::tile_type &pos)
		{
			pos.setCenter(pattern->center);
			pos.setRing(pattern->externalRing);
		}

#pragma region Path
	private:
		auto getAngles(const typename Grid::tile_type &referenceTile, const vector<typename Grid::tile_ptr> &neighbors)
		{
			vector<float> angles;

			for (const auto& neighbor : neighbors)
			{
				angles.push_back(referenceTile.getAngle(*neighbor));
			}

			return angles;
		}

		auto getRelativeAngles(float referenceAngle, const vector<float> &angles)
		{
			vector<float> relativeAngles;

			for (float angle: angles)
			{
				relativeAngles.push_back(Math::getAngularDistance(referenceAngle, angle));
			}

			return relativeAngles;
		}

		void filterNeighbors(vector<typename Grid::tile_ptr> &neighbors, vector<float>& angles, vector<float>& relativeAngles, float threshold = 2 * static_cast<float>(std::_Pi))
		{
			for (int i = neighbors.size() - 1; i != -1; --i)
			{
				if(relativeAngles[i] > threshold)
				{
					neighbors.erase(neighbors.begin() + i);
					angles.erase(angles.begin() + i);
					relativeAngles.erase(relativeAngles.begin() + i);
				}
			}
		}

		auto getTileProbability(vector<typename Grid::tile_ptr> &pool, vector<float> &relativeAngles)
		{
			vector<float> probability;

			float range = 0.0f;

			for(size_t i = 0; i != relativeAngles.size(); ++i)
			{
				for (size_t j = i; j != relativeAngles.size(); ++j)
				{
					range = max(range, Math::getAngularDistance(relativeAngles[i], relativeAngles[j]));
				}
			}

			for (size_t i = 0; i != relativeAngles.size(); ++i)
			{
				probability.push_back((range - relativeAngles[i]) / (range * (static_cast<float>(relativeAngles.size()) - 1)));
			}

			return probability;
		}

		typename Grid::tile_type& getNextTilePath(Grid &grid, typename Grid::tile_type &current, const typename Grid::tile_type &goal)
		{
			vector<typename Grid::tile_ptr> neighbors =  grid.getNeighbors(current);

			vector<float> angles = getAngles(current, neighbors);

			const float referenceAngle = current.getAngle(goal);
			vector<float> relativeAngles = getRelativeAngles(referenceAngle, angles);

			filterNeighbors(neighbors, angles, relativeAngles, 2 * static_cast<float>(std::_Pi));

			vector<float> probability = getTileProbability(neighbors, relativeAngles);

			std::discrete_distribution<> distribution(probability.begin(), probability.end());

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
			pattern_ptr pattern = getPattern(tagStart);
			setPatternAt(pattern, start);

			typename Grid::tile_type current = start;

			while (current != goal)
			{
				current = getNextTilePath(grid, current, goal);

				pattern_ptr pattern = getPatternAt(tagStart);
			}
			
		}

		void buildPath(Grid& grid, tag_type tagStart, tag_type tagGoal,	tag_type tagPath)
		{
			buildPath(grid, std::move(tagStart), std::move(tagGoal), std::move(tagPath), getRandomPos(grid), getRandomPos(grid));
		}

	};
}
