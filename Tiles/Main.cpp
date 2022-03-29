#include <string>

#include "printer.h"
#include "ProceduralGeneration/deserializer.h"
#include "ProceduralGeneration/Generation.h"

using namespace ProceduralGen;
using namespace shin_grid;

int main()
{
	GenerationData genData;
	deserialize(std::string{ "Tuiles.txt" }, genData);

	RandomGenerator randomGenerator{ 4 };

	Hexagonal::Grid grid{17, 13};

	PartialGrid<Hexagonal::Grid>::mask_t layout =
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

	PartialGrid<Hexagonal::Grid> partialGrid {grid, layout};

	GenerationProcess gen(genData, randomGenerator, partialGrid);
	gen.resetGrid();

	auto condition = [](Hexagonal::Grid::tile_ptr current)
	{
		std::vector<string> tags = { "Path", "PathEnd" };
		return VectorMath::containsAny(current->getPattern().tags, tags);
	};

	try
	{
		gen.buildPath({ partialGrid.getValidTile(0), partialGrid.getValidTile(140), "PathEnd", "PathEnd", "Path", 'C' });
		gen.buildPath({ partialGrid.getValidTile(37), partialGrid.getValidTile(117), "PathEnd", "PathEnd", "Path", 'C' });
		gen.buildPath({ nullptr, nullptr, "PathEnd", "PathEnd", "Path", 'C', condition}, condition, condition);
		

		gen.buildRiver({"RiverEnd",	"RiverEnd",	"River",	nullptr,	'R',	30	});


		// todo : some constrainsts are left over
		gen.populate({  "Void", "Mine", "Building"});
	}
	catch(...)
	{
		printer::print("log.txt", partialGrid);
		throw std::exception{};
	}

	printer::print("output.txt", partialGrid);
}