#pragma once
#include <vector>
#include "../Tile.h"
#include "../ShinMathLib/VectorMath.h"

namespace Hexagonal {
	using namespace std;

	using Tile = ProceduralGenerationImplementation::Tile;

	class Grid
	{
	public:
		using tile_t = Tile;
		using tile_ptr = tile_t*;

		using index_t = size_t;

	private:
		index_t width;
		index_t height;

	public:
		vector<tile_ptr> grid{ width * height };

	public:
		Grid(index_t width = 17, index_t height = 13) : width {width}, height{height}, grid{ width * height }
		{
			grid.reserve(getSize());
			for (size_t i = 0; i != getHeight(); ++i)
			{
				for (size_t j = 0; j != getWidth(); ++j)
				{
					grid[getIndex(i, j)] = new tile_t{ Coord::getCoord(i, j) };
				}
			}
		}

		~Grid()
		{
			for (size_t i = 0; i != getHeight(); ++i)
			{
				for (size_t j = 0; j != getWidth(); ++j)
				{
					delete grid[getIndex(i, j)];
				}
			}
		}		
	
	private:
		index_t getI(index_t index) const
		{
			return index / getWidth();
		}

		index_t getJ(index_t index) const
		{
			return index - getI(index) * getWidth();
		}

		bool areValidCoordinates(index_t i, index_t j) const
		{
			return 0 <= i && i < getHeight()
				&& 0 <= j && j < getWidth();
		}

	public:
		index_t getWidth() const noexcept
		{
			return width;
		}

		index_t getHeight() const noexcept
		{
			return height;
		}

		index_t getSize() const noexcept
		{
			return width * height;
		}

		index_t getIndex(const tile_t& tile) const
		{
			return tile.getQ() + tile.getR() * getWidth() + tile.getR() / 2;
		}

		index_t getIndex(index_t i, index_t j) const
		{
			return i * getWidth() + j;
		}


		tile_ptr getTile(index_t index) const
		{
			return grid[index];
		}


		vector<tile_ptr> getTiles()
		{
			return grid;
		}


#pragma region Validation
		/**
		 * \brief Careful, if you modify a valid index then check if the new index is valid, you need to check if the new value didn't overflow size_type
		 */
		bool isValidIndex(index_t index) const
		{
			return 0 <= index && index < getSize();
		}
#pragma endregion

#pragma region Relationships
		vector<index_t> getNeighborsIndex(index_t index) const
		{
			vector<index_t> v{};

			size_t i = getI(index);
			size_t j = getJ(index);

			bool evenRow = i % 2 == 0;

			bool eastLimit = 0 == j;
			bool westLimit = j == getWidth() - 1;
			bool northLimit = 0 == i;
			bool southLimit = i == getHeight() - 1;
			bool southEastLimit = southLimit || (evenRow && eastLimit);
			bool southWestLimit = southLimit || (!evenRow && westLimit);
			bool northEastLimit = northLimit || (evenRow && eastLimit);
			bool northWestLimit = northLimit || (!evenRow && westLimit);

			size_t west = index + 1;
			size_t south_west = index + getWidth() + !evenRow;
			size_t south_east = index + getWidth() - evenRow;
			size_t east = index - 1;
			size_t north_east = index - getWidth() - evenRow;
			size_t north_west = index - getWidth() + !evenRow;

			if (!westLimit) v.push_back(west);
			if (!southWestLimit) v.push_back(south_west);
			if (!southEastLimit) v.push_back(south_east);
			if (!eastLimit) v.push_back(east);
			if (!northEastLimit) v.push_back(north_east);
			if (!northWestLimit) v.push_back(north_west);

			return v;
		}

		vector<tile_ptr> getNeighbors(const tile_t& tile) const
		{
			const vector<index_t> v = getNeighborsIndex(getIndex(tile));
			vector<tile_ptr> neighbors;
			for (const auto index : v)
			{
				neighbors.push_back(grid[index]);
			}
			return neighbors;
		}
#pragma endregion
	};
}
