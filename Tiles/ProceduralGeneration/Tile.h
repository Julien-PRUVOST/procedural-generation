#pragma once
#include <string>
#include <vector>

namespace ProceduralGeneration
{
	using namespace std;

	class Tile
	{
	public:
		using tag_type = string;
		using tile_element_type = char;

		tag_type tag{};
		vector<tile_element_type> constraintsRing {};
		vector<tile_element_type> externalRing {};
		tile_element_type center {};
	};
}
