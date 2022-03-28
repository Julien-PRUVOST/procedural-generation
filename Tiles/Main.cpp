#include <string>

#include "printer.h"
#include "ProceduralGeneration/deserializer.h"
#include "ProceduralGeneration/Generation.h"

using namespace ProceduralGen;

int main()
{
	GenerationProcess<Pattern<Element<char>>> gen(4);
	deserialize(std::string{ "Tuiles.txt" }, gen);

	auto condition = [](const Hexagonal::Grid::tile_ptr& current)
	{
		std::vector<string> tags = { "Path", "PathEnd" };
		return VectorMath::containsAny(current->getTags(), tags);
	};

	Hexagonal::Grid grid {};

	grid.clear();

	try
	{
		gen.buildPath(grid, "PathEnd", "PathEnd", "Path", grid.getValidTile(0), grid.getValidTile(140), 'C');
		gen.buildPath(grid, "PathEnd", "PathEnd", "Path", grid.getValidTile(37), grid.getValidTile(117), 'C');
		gen.buildPath(grid, "PathEnd", "PathEnd", "Path", 'C', nullptr, nullptr, condition, condition, condition);
		/*gen.buildPath(grid, "PathEnd", "PathEnd", "Path", 'C', nullptr, nullptr, condition, condition, condition);
		gen.buildPath(grid, "PathEnd", "PathEnd", "Path", 'C', nullptr, nullptr, condition, condition, condition);
		gen.buildPath(grid, "PathEnd", "PathEnd", "Path", 'C', nullptr, nullptr, condition, condition, condition);*/

		gen.buildRiver(grid, "RiverEnd", "RiverEnd", "River", 'R', 30);
		// gen.populate(grid, {  "Void" });
	}
	catch(...)
	{
		printer::print("log.txt", grid);
		throw std::exception{};
	}

	printer::print("output.txt", grid);
}