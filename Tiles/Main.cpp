#include <string>

#include "ProceduralGeneration/deserializer.h"
#include "ProceduralGeneration/Generation.h"

using namespace ProceduralGeneration;

int main()
{
	GenerationProcess gen;
	deserialize(string{ "Tuiles.txt" }, gen);

	Hexagonal::Grid grid {};

	gen.buildPath(grid, "River", "River", grid.getTile(0), grid.getTile(60), 'C');
}