#include "ShinHexGrid.h"

#include "hex-coords.h"

namespace ProceduralGeneration {
	HexGrid::HexGrid()
	{
		layout.reserve(gridWidth * gridHeight);
	}

	void HexGrid::createGrid()
	{
		grid.clear();

		for (coords_type i = 0; i != gridHeight; ++i)
		{
			for (coords_type j = 0; j != gridWidth; ++j)
			{
				if (getLayout(i * gridWidth + j))
				{
					grid.emplace_back(HexCoords::getQ(i, j), HexCoords::getR(i, j), d6(prng_generation));
				}
				else
				{
					grid.emplace_back(HexCoords::getQ(i, j), HexCoords::getR(i, j));
				}
			}
		}
	}

	void HexGrid::createRandomGrid()
	{
		setRandomSeed();
		createGrid();
	}
}