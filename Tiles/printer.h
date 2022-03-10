#pragma once

#include <fstream>
#include <iostream>

#include "Hexagonal/Grid.h"
#include "Tile.h"

namespace printer
{
	inline void print(std::ostream& os, ProceduralGeneration::element_t element, ProceduralGeneration::element_t default_appearance = '.')
	{
		os << (element == ProceduralGeneration::element_t{} || element == '0' ? default_appearance : element);
	}

	inline void print(std::ostream& os, Hexagonal::Grid &grid)
	{
		for (size_t i = 0; i != grid.getHeight(); ++i)
		{
			if (i % 2 == 1)
				os << "   ";

			os << " ";
			for (size_t j = 0; j != grid.getWidth(); ++j)
			{
				size_t index = grid.getIndex(i, j);
				const auto tile = grid.getTile(index);
				bool valid = grid.getLayout(index);

				print(os, tile->getPattern().externalRing[4], valid ? '.' : ' ');
				os << " ";
				print(os, tile->getPattern().externalRing[5], valid ? '.' : ' ');
				os << "   ";
			}

			os << std::endl;
			if (i % 2 == 1)
				os << "   ";

			for (size_t j = 0; j != grid.getWidth(); ++j)
			{
				size_t index = grid.getIndex(i, j);
				const auto tile = grid.getTile(index);
				bool valid = grid.getLayout(index);

				print(os, tile->getPattern().externalRing[3], valid ? '.' : ' ');
				os << " ";
				print(os, tile->getPattern().center, valid ? '.' : ' ');
				os << " ";
				print(os, tile->getPattern().externalRing[0], valid ? '.' : ' ');
				os << " ";
			}

			os << std::endl;
			if (i % 2 == 1)
				os << "   ";

			os << " ";
			for (size_t j = 0; j != grid.getWidth(); ++j)
			{
				size_t index = grid.getIndex(i, j);
				const auto tile = grid.getTile(index);
				bool valid = grid.getLayout(index);

				print(os, tile->getPattern().externalRing[2], valid ? '.' : ' ');
				os << " ";
				print(os, tile->getPattern().externalRing[1], valid ? '.' : ' ');
				os << "   ";
			}
			os << std::endl;
		}
	}

	inline void print(const std::string& filename, Hexagonal::Grid& grid)
	{
		std::ofstream file{ filename };
		print(file, grid);
	}

	

	

}