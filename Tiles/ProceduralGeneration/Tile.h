#pragma once
#include <sstream>
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


#pragma region Deserialization

	inline void deserialize(const string& line, unique_ptr<Tile>& tile)
	{
		istringstream input{ line };

		string subLine;

		getline(input, subLine, ' ');
		tile->tag = subLine;

		for (int i = 0; i < 6 && getline(input, subLine, ' '); ++i)
		{
			tile->constraintsRing.push_back(subLine[0]);
		}

		for (int i = 0; i < 6 && getline(input, subLine, ' '); ++i)
		{
			tile->externalRing.push_back(subLine[0]);
		}

		getline(input, subLine, ' ');
		tile->center = subLine[0];
	}

#pragma endregion
}
