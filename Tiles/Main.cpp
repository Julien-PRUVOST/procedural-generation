#include <string>

#include "printer.h"
#include "ProceduralGeneration/deserializer.h"
#include "ProceduralGeneration/Generation.h"

using namespace ProceduralGen;

int main()
{
	GenerationProcess<Pattern<Element<char>>> gen;
	deserialize(std::string{ "Tuiles.txt" }, gen);

	Hexagonal::Grid grid {};

	grid.clear();

	gen.buildPath(grid, "RiverEnd", "RiverEnd", "River", grid.getValidTile(0), grid.getValidTile(140), 'R');
	printer::print("output.txt", grid);
}