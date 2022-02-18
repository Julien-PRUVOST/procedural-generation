#pragma once
#include <vector>
#include <memory>
#include <random>
#include <map>

namespace ProceduralGeneration
{
	using namespace std;

	template<class Tile, class Rule = void>
	class GenerationProcess
	{
	public:
		using rule_t = Rule;
		using tile_t = Tile;
		using rule_ptr = unique_ptr<Rule>;
		using tile_ptr = unique_ptr<Tile>;

		using tag_type = typename Tile::tag_type;

	private:
		vector<rule_ptr> rules;
		map<tag_type, vector<tile_ptr>> tiles {};

		/**
		 * \brief The reproductibility of the seed cannot be guaranteed if multiple generation commands are used simultaneously on the same GenerationProcess
		 */
		mt19937 prng;

	public:
		void add(rule_ptr &&rule)
		{
			rules.push_back(std::move(rule));
		}

		void add(tile_ptr &&tile)
		{
			tiles[tile->tag].push_back(std::move(tile));
		}

	public:
		template <class Grid>
		void populate(Grid &grid)
		{
			
		}

	public:
		/**
		 * \brief Is public only for debug puposes
		 * \return A random position on the grid
		 */
		template <class Grid>
		typename Grid::tile_type& getRandomPos(Grid grid)
		{
			uniform_int_distribution<typename Grid::coords_type> posChooser{ 0, grid.getSize() };

			return grid.getTile(posChooser(prng));
		}

		template <class Grid>
		void buildPath(
			Grid grid,
			tag_type tagStart,
			tag_type tagGoal,
			tag_type tagPath,
			typename Grid::tile_type &start = getRandomPos(grid),
			typename Grid::tile_type &goal = getRandomPos(grid))
		{
			
		}
	};
}
