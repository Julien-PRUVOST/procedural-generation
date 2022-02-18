#pragma once
#include <random>
#include <vector>
#include <numeric>
#include <memory>

#include "ShinHexTile.h"

namespace ProceduralGeneration {
	using namespace std;

	class HexGrid
	{
	public:
		using coords_type = size_t;
		using seed_type = int;
		using mask_type = std::vector<bool>;

		using tile_ptr = shared_ptr<HexTile>;

	public:
		HexGrid();

	private:
		coords_type gridWidth = 17;
		coords_type gridHeight = 13;

		mask_type layout =
		{
			0, 0, 0, 0, 0, 0, 0, 1,	1, 1, 1, 1, 1, 1, 1, 1, 1,      // 0 
			  0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,	// 1
			0, 0, 1, 1, 1, 1, 1, 1,	1, 1, 1, 1, 1, 1, 1, 0, 0,		// 2
			  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,	// 3
			1, 1, 1, 1, 0, 0, 1, 1,	1, 1, 1, 1,	1, 0, 1, 1, 1,		// 4
			  1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,	// 5
			1, 1, 1, 1, 1, 1, 1, 1,	1, 1, 1, 1,	1, 1, 1, 1, 0,		// 6 --
			  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,	// 7
			0, 1, 1, 1, 0, 0, 1, 1,	1, 1, 1, 1,	1, 0, 0, 0, 0,		// 8
			  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,	// 9
			0, 0, 1, 1, 1, 1, 1, 1,	1, 1, 1, 1,	1, 1, 1, 0, 0,		// 10
			  0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,	// 11
			0, 0, 0, 0, 0, 0, 0, 0,	1, 1, 1, 1,	0, 1, 1, 1, 0,		// 12
		};

		/**
		 * \brief Seed used for procedural generation
		 */
		seed_type seed = 0;

		std::mt19937 prng_generation{ std::random_device{}() };
		std::uniform_int_distribution<> d6{ 0,5 };

		std::mt19937 prng_seed{ std::random_device{}() };
		std::uniform_int_distribution<> seedDice{ 0, std::max(std::numeric_limits<int>::max(), std::numeric_limits<seed_type>::max()) };

	private:
		coords_type getIndex(coords_type i, coords_type j) const
		{
			return i * getGridWidth() + j;
		}

	public:
		std::vector<tile_ptr> grid;

	public:
		coords_type getGridWidth() const noexcept
		{
			return gridWidth;
		}

		coords_type getGridHeight() const noexcept
		{
			return gridHeight;
		}

		coords_type getSize() const noexcept
		{
			return gridWidth * gridHeight;
		}

		bool getLayout(size_t index) const
		{
			return layout[index];
		}

		bool getLayout(coords_type i, coords_type j) const
		{
			return getLayout(getIndex(i, j));
		}

		tile_ptr getTile(coords_type index)
		{
			return grid[index];
		}

		const tile_ptr getTile(coords_type index) const
		{
			return grid[index];
		}

		tile_ptr getTile(coords_type i, coords_type j)
		{
			return getTile(getIndex(i, j));
		}

		const tile_ptr getTile(coords_type i, coords_type j) const
		{
			return getTile(getIndex(i, j));
		}

		seed_type getSeed() const noexcept
		{
			return seed;
		}		

		void setSeed(seed_type newSeed)
		{
			seed = newSeed;
			prng_generation.seed(seed);
		}

		void setRandomSeed()
		{
			setSeed(seedDice(prng_seed));
		}

	public:
		void setLayout(coords_type width, coords_type height, mask_type newLayout)
		{
			gridWidth = width;
			gridHeight = height;
			layout.swap(newLayout);
		}

	public:
		void createGrid();
		void createRandomGrid();
	};
}