#include <string>

#include "ProceduralGeneration/deserializer.h"
#include "ProceduralGeneration/Generation.h"
#include "ProceduralGeneration/Tile.h"
#include "ProceduralGeneration/Rule.h"
#include "Hexagonal/Grid.h"

using namespace ProceduralGeneration;

int main()
{
	GenerationProcess<Tile, Rule> gen;
	deserialize(string{ "Tuiles.txt" }, gen);

	HexGrid grid{};
	// gen.buildPath(grid, start, end, tagStart, tagEnd, tagPath);
}