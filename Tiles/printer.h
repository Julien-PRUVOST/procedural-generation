#pragma once

#include <fstream>
#include <iostream>

#include "Hexagonal/Grid.h"
#include "Tile.h"

namespace printer
{
	template <class T>
	void print(std::ostream& os, const ProceduralGen::Element<T>& element, const T& defaultDisplay = '.') {
		os << (element.isDefault() ? defaultDisplay : element.value());
	};

	template <class PartialGrid>
	void print(std::ostream& os, PartialGrid& grid)
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
				bool valid = grid.isValid(index);

				if (valid)
				{
					print(os, tile->getPattern().data[0][4], '.');
					os << " ";
					print(os, tile->getPattern().data[0][5], '.');
					os << "   ";
				}
				else
				{
					print(os, {}, ' ');
					os << " ";
					print(os, {}, ' ');
					os << "   ";
				}
				
			}

			os << std::endl;
			if (i % 2 == 1)
				os << "   ";

			for (size_t j = 0; j != grid.getWidth(); ++j)
			{
				size_t index = grid.getIndex(i, j);
				const auto tile = grid.getTile(index);
				bool valid = grid.isValid(index);
				if (valid)
				{
					print(os, tile->getPattern().data[0][3],'.');
					os << " ";
					print(os, tile->getPattern().data[1][0],'•');
					os << " ";
					print(os, tile->getPattern().data[0][0],'.');
					os << " ";
				} else
				{
					print(os, {}, ' ');
					os << " ";
					print(os, {}, ' ');
					os << " ";
					print(os, {}, ' ');
					os << " ";
				}
				
			}

			os << std::endl;
			if (i % 2 == 1)
				os << "   ";

			os << " ";
			for (size_t j = 0; j != grid.getWidth(); ++j)
			{
				size_t index = grid.getIndex(i, j);
				const auto tile = grid.getTile(index);
				bool valid = grid.isValid(index);

				if (valid)
				{
					print(os, tile->getPattern().data[0][2], '.');
					os << " ";								 
					print(os, tile->getPattern().data[0][1], '.');
					os << "   ";
				}
				else
				{
					print(os, {}, ' ');
					os << " ";
					print(os, {}, ' ');
					os << "   ";
				}
				
			}
			os << std::endl;
		}
	}

	template <class PartialGrid>
	void print(const std::string& filename, PartialGrid& grid)
	{
		std::ofstream file{ filename };
		print(file, grid);
	}


	template <class PartialGrid>
	void printConstraints(std::ostream& os, PartialGrid& grid)
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
				bool valid = grid.isValid(index);

				if (valid)
				{
					print(os, tile->getPattern().constraints[0][4], '.');
					os << " ";
					print(os, tile->getPattern().constraints[0][5], '.');
					os << "   ";
				}
				else
				{
					print(os, {}, ' ');
					os << " ";
					print(os, {}, ' ');
					os << "   ";
				}

			}

			os << std::endl;
			if (i % 2 == 1)
				os << "   ";

			for (size_t j = 0; j != grid.getWidth(); ++j)
			{
				size_t index = grid.getIndex(i, j);
				const auto tile = grid.getTile(index);
				bool valid = grid.isValid(index);
				if (valid)
				{
					print(os, tile->getPattern().constraints[0][3], '.');
					os << " ";
					print(os, {}, '•');
					os << " ";
					print(os, tile->getPattern().constraints[0][0], '.');
					os << " ";
				}
				else
				{
					print(os, {}, ' ');
					os << " ";
					print(os, {}, ' ');
					os << " ";
					print(os, {}, ' ');
					os << " ";
				}

			}

			os << std::endl;
			if (i % 2 == 1)
				os << "   ";

			os << " ";
			for (size_t j = 0; j != grid.getWidth(); ++j)
			{
				size_t index = grid.getIndex(i, j);
				const auto tile = grid.getTile(index);
				bool valid = grid.isValid(index);

				if (valid)
				{
					print(os, tile->getPattern().constraints[0][2], '.');
					os << " ";
					print(os, tile->getPattern().constraints[0][1], '.');
					os << "   ";
				}
				else
				{
					print(os, {}, ' ');
					os << " ";
					print(os, {}, ' ');
					os << "   ";
				}

			}
			os << std::endl;
		}
	}

	template <class PartialGrid>
	void printConstraints(const std::string& filename, PartialGrid& grid)
	{
		std::ofstream file{ filename };
		printConstraints(file, grid);
	}

	

	

}
