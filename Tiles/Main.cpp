#include <string>

#include "printer.h"
#include "ProceduralGeneration/deserializer.h"
#include "ProceduralGeneration/Generation.h"

using namespace ProceduralGeneration;

int main()
{
	GenerationProcess gen;
	deserialize(string{ "Tuiles.txt" }, gen);

	Hexagonal::Grid grid {};

	grid.clear();

	gen.buildPath(grid, "RiverEnd", "RiverEnd", "River", grid.getValidTile(0), grid.getValidTile(140), 'R');
	printer::print("output.txt", grid);
}