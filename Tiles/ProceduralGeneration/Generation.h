#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <memory>

namespace ProceduralGeneration
{
	using namespace std;

	template<class Grid, class Tile, class Rule = void>
	class Generation
	{
		vector<unique_ptr<Rule>> rules;
		vector<unique_ptr<Tile>> tiles;

	public:
		void add(unique_ptr<Rule> && rule)
		{
			rules.push_back(move(rule));
		}

		void add(unique_ptr<Tile> && tile)
		{
			tiles.push_back(move(tile));
		}
	};

#pragma region Deserialization
	
	template <class T, class Generation>
	void deserializeCategory(ifstream& file, string& line, Generation& gen)
	{
		while (getline(file, line) && line.find("#", 0) != 0)
		{
			if (line.empty()) continue;
			auto t = make_unique<T>();
			deserialize(line, t);
			gen.add(move(t));
		}
	}

	template<class Tile, class Rule = void, class Generation>
	void deserialize(string filename, Generation& gen)
	{
		ifstream file{ filename };
		if (file)
		{
			string line;
			if (!getline(file, line)) return;

			if (line.find("# Rules", 0) == 0)
			{
				deserializeCategory<Rule>(file, line, gen);
			}

			if (line.find("# Encoding", 0) == 0)
			{
				deserializeCategory<Tile>(file, line, gen);
			}
		}
	}

#pragma endregion
}
