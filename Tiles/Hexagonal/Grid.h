#pragma once
#include <vector>
#include <memory>
#include "../Tile.h"
#include "../ShinMathLib/VectorMath.h"

namespace Hexagonal {
	using namespace std;

	using Tile = ProceduralGenerationImplementation::Tile<ProceduralGen::Pattern<ProceduralGen::Element<char>>>;
	// template <class Tile>
	class Grid
	{
	public:
		using tile_type = Tile;
		using tile_ptr = tile_type*;

		using size_type = size_t;

		using mask_type = vector<bool>;

	public:
		Grid()
		{
			grid.reserve(getSize());
			for (size_t i = 0; i != getHeight(); ++i)
			{
				for (size_t j = 0; j != getWidth(); ++j)
				{
					grid[getIndex(i, j)] = new tile_type{ Coord::getCoord(i, j) };
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
		size_type width = 17;
		size_type height = 13;

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

		size_t validTileSize = shinmathlib::VectorMath::sum(layout, size_t{});

	public:
		vector<tile_ptr> grid{ width * height };

	private:
		size_type getI(size_type index) const
		{
			return index / getWidth();
		}

		size_type getJ(size_type index) const
		{
			return index - getI(index) * getWidth();
		}

		bool areValidCoordinates(size_type i, size_type j) const
		{
			return 0 <= i && i < getHeight()
				&& 0 <= j && j < getWidth();
		}

	public:
		size_type getWidth() const noexcept
		{
			return width;
		}

		size_type getHeight() const noexcept
		{
			return height;
		}

		size_type getSize() const noexcept
		{
			return width * height;
		}

		size_type getIndex(const tile_type& tile) const
		{
			return tile.getQ() + tile.getR() * getWidth() + tile.getR() / 2;
		}

		size_type getIndex(size_type i, size_type j) const
		{
			return i * getWidth() + j;
		}

		bool getLayout(size_type index) const
		{
			return layout[index];
		}

		mask_type getLayout() const
		{
			return layout;
		}

		tile_ptr getTile(size_type index) const
		{
			return grid[index];
		}

		size_t getValidTileSize() const
		{
			return validTileSize;
		}

		vector<tile_ptr> getValidTiles() const
		{
			vector<tile_ptr> validTiles;

			for (size_type i = 0; i != getSize(); ++i)
			{
				if (getLayout(i)) validTiles.push_back(getTile(i));
			}

			return validTiles;
		}

		tile_ptr getValidTile(size_type index) const
		{
			size_t count = -1;
			size_t i;
			for (i = 0; count != index; ++i)
			{
				if (getLayout(i)) ++count;
			}

			return getTile(i - 1);
		}

		vector<tile_ptr> getTiles()
		{
			return grid;
		}

		void clear()
		{
			for (auto tile : grid)
			{
				tile->erase();
			}
		}


#pragma region Validation
		/**
		 * \brief Careful, if you modify a valid index then check if the new index is valid, you need to check if the new value didn't overflow size_type
		 */
		bool isValidIndex(size_type index) const
		{
			return 0 <= index && index < getSize();
		}
#pragma endregion

#pragma region Relationships
		vector<size_type> getNeighborsIndex(size_type index) const
		{
			vector<size_type> v{};

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

		vector<size_type> getValidNeighborsIndex(size_type index) const
		{
			vector<size_type> v{};

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

			if (!westLimit && getLayout(west)) v.push_back(west);
			if (!southWestLimit && getLayout(south_west)) v.push_back(south_west);
			if (!southEastLimit && getLayout(south_east)) v.push_back(south_east);
			if (!eastLimit && getLayout(east)) v.push_back(east);
			if (!northEastLimit && getLayout(north_east)) v.push_back(north_east);
			if (!northWestLimit && getLayout(north_west)) v.push_back(north_west);

			return v;
		}

		vector<tile_ptr> getNeighbors(const tile_type& tile) const
		{
			const vector<size_type> v = getNeighborsIndex(getIndex(tile));
			vector<tile_ptr> neighbors;
			for (const auto index : v)
			{
				neighbors.push_back(grid[index]);
			}
			return neighbors;
		}

		vector<tile_ptr> getValidNeighbors(const tile_type& tile) const
		{
			const vector<size_type> v = getValidNeighborsIndex(getIndex(tile));
			vector<tile_ptr> neighbors;
			for (const auto index : v)
			{
				neighbors.push_back(grid[index]);
			}
			return neighbors;
		}
#pragma endregion

		

	public:
		void setLayout(size_type width_, size_type height_, mask_type newLayout)
		{
			width = width_;
			height = height_;
			layout.swap(newLayout);
			validTileSize = shinmathlib::VectorMath::sum(layout, size_t{});
		}
	};
}
