#ifndef PARTIAL_GRID_H
#define PARTIAL_GRID_H

#include <vector>

#include "../ShinMathLib/VectorMath.h"

namespace shin_grid
{
	using namespace std;
	using namespace shinmathlib;
	namespace VM = VectorMath;

	template <class Grid>
	class PartialGrid
	{
		using tile_t = typename Grid::tile_t;

	public:
		using tile_ptr = typename Grid::tile_ptr;
		using mask_t = vector<bool>;
		using index_t = typename Grid::index_t;

	private:
		Grid& grid;
		mask_t layout;

		index_t sizeValidTiles = VM::sum(layout, index_t{ 0 });

		index_t getGridIndex(index_t validIndex) const
		{
			index_t count = 0;
			index_t i = 0;
			for (i = 0; count != validIndex + 1; ++i)
			{
				if (isValid(i)) ++count;
			}
			return i;
		}

	public:
		PartialGrid(Grid& grid, mask_t layout) : grid{grid}, layout { layout }
		{
		}

		index_t getHeight()
		{
			return grid.getHeight();
		}

		index_t getWidth()
		{
			return grid.getWidth();
		}

		index_t getIndex(index_t i, index_t j)
		{
			return grid.getIndex(i, j);
		}

		tile_ptr getTile(index_t gridIndex)
		{
			return grid.getTile(gridIndex);
		}


		bool isValid(index_t gridIndex) const
		{
			return layout[gridIndex];
		}

		mask_t getLayout() const
		{
			return layout;
		}

		index_t getValidTilesSize() const
		{
			return sizeValidTiles;
		}

		vector<tile_ptr> getValidTiles() const
		{
			vector<tile_ptr> validTiles;

			for (index_t index = 0; index != grid.getSize(); ++index)
			{
				if (isValid(index)) validTiles.push_back(grid.getTile(index));
			}

			return validTiles;
		}

		tile_ptr getValidTile(index_t validIndex) const
		{
			return grid.getTile(getGridIndex(validIndex));
		}

		vector<tile_ptr> getValidNeighbors(index_t validIndex) const
		{
			vector<index_t> neighborsIndex = grid.getNeighborsIndex(getGridIndex(validIndex));
			vector<tile_ptr> result;

			for (index_t i : neighborsIndex)
			{
				if (isValid(i)) result.push_back(grid.getTile(i));
			}

			return result;
		}

		vector<tile_ptr> getValidNeighbors(tile_ptr tile)
		{
			vector<index_t> neighborsIndex = grid.getNeighborsIndex(grid.getIndex(*tile));
			vector<tile_ptr> result;

			for (index_t i : neighborsIndex)
			{
				if (isValid(i)) result.push_back(grid.getTile(i));
			}

			return result;
		}
	};
}


#endif // PARTIAL_GRID_H
