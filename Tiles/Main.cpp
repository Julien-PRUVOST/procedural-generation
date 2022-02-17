#include <string>
#include "ProceduralGeneration/Generation.h"
#include "ProceduralGeneration/Tile.h"
#include "ProceduralGeneration/Rule.h"
#include "ProceduralGeneration/ShinHexGrid.h"

using namespace ProceduralGeneration;

int main()
{
	Generation<HexGrid, Tile, Rule> gen;
	deserialize<Tile, Rule>(string{ "Tuiles.txt" }, gen);
}