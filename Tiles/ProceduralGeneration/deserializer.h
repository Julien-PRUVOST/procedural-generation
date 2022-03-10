#pragma once
#include <fstream>
#include <sstream>
#include <string>

namespace ProceduralGeneration
{
	using std::string;

	inline bool decodeAsInt(const string& word, char& c, unsigned int& n)
	{
		try
		{
			n = stoul(word);
			return true;
		}
		catch (...)
		{
			c = word[0];
			return false;
		}
	}

	template <class Generation>
	void deserialize(const string &line, typename Generation::pattern_t &tile)
	{
		std::istringstream input{ line };

		string subLine;

		getline(input, subLine, ' ');
		tile.tag = subLine;

		for (int i = 0; i < 6 && getline(input, subLine, ' '); ++i)
		{
			tile.constraintsRing.push_back(subLine[0]);
		}

		for (int i = 0; i < 6 && getline(input, subLine, ' '); ++i)
		{
			tile.externalRing.push_back(subLine[0]);
		}

		getline(input, subLine, ' ');
		tile.center = subLine[0];

		getline(input, subLine, ' ');
		char unused_c;
		unsigned int n;
		decodeAsInt(subLine, unused_c, n);
		tile.weight = static_cast<size_t>(n);
	}

#pragma region Generation

	template <class T, class Generation>
	void deserializeCategory(std::ifstream& file, string& line, Generation& gen)
	{
		while (getline(file, line) && line.find("#", 0) != 0)
		{
			if (line.empty()) continue;
			T t;
			deserialize<Generation>(line, t);
			gen.add(std::move(t));
		}
	}

	template<class Generation>
	void deserialize(string filename, Generation& gen)
	{
		std::string s = "MyUniqueFileTalkingAboutPengouins.txt";
		std::ofstream f{ s };

		std::ifstream file{ filename };
		if (file)
		{
			string line;

			deserializeCategory<Generation::pattern_t>(file, line, gen);
		}
	}
#pragma endregion
}