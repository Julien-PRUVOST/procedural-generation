#pragma once
#include <fstream>
#include <sstream>
#include <string>

namespace ProceduralGen
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

		tile.constraints.push_back({});
		for (int i = 0; i < 6 && getline(input, subLine, ' '); ++i)
		{
			tile.constraints[0].push_back(subLine[0]);
		}

		tile.data.push_back({});

		for (int i = 0; i < 6 && getline(input, subLine, ' '); ++i)
		{
			tile.data[0].push_back(subLine[0]);
		}

		getline(input, subLine, ' ');
		tile.data.push_back({});
		tile.data[1].push_back(subLine[0]);

		getline(input, subLine, ' ');
		char unused_c;
		unsigned int n;
		decodeAsInt(subLine, unused_c, n);
		tile.weight = static_cast<size_t>(n);
	}

	template <class T>
	void deserializeDefaultElements(std::ifstream& file, string& line)
	{
		while (getline(file, line) && line.find('#', 0) != 0)
		{
			if (line.empty()) continue;

			std::istringstream input{ line };
			string subLine;

			while (std::getline(input, subLine, ' '))
			{
				Element<T>::addDefaultValue(T{ subLine[0] });
			}
		}
	}

#pragma region Generation

	template <class T, class Generation>
	void deserializeCategory(std::ifstream& file, string& line, Generation& gen)
	{
		while (getline(file, line) && line.find('#', 0) != 0)
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
		std::ifstream file{ filename };
		if (file)
		{
			string line;

			if (!getline(file, line)) return;

			if (line.find("# Default", 0) == 0)
			{
				deserializeDefaultElements<char>(file, line);
			}

			if (line.find("# Encoding", 0) == 0)
			{
				deserializeCategory<Generation::pattern_t>(file, line, gen);
			}
		}
	}
#pragma endregion
}