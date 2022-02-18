#pragma once
#include <fstream>
#include <sstream>
#include <string>

namespace ProceduralGeneration
{
	using namespace std;

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
	void deserialize(const string &line, typename Generation::rule_ptr &rule)
	{
		istringstream input{ line };

		string subLine;
		bool mask[3] = { true, true, true };
		typename Generation::rule_t::tile_element_type params[3];
		typename Generation::rule_t::value_type n = 0;

		for (int i = 0; i < 3 && getline(input, subLine, ' '); ++i)
		{
			mask[i] = decodeAsInt(subLine, params[i], n);
		}

		if (!mask[0] && mask[1] && mask[2]) rule->create(params[0], n);
		if (!mask[0] && !mask[1] && mask[2]) rule->create(params[0], params[1], n);
		if (!mask[0] && !mask[1] && !mask[2]) rule->create(params[0], params[1], params[2]);
	}


	template <class Generation>
	void deserialize(const string &line, typename Generation::tile_ptr &tile)
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

#pragma region Generation

	template <class T, class Generation>
	void deserializeCategory(ifstream& file, string& line, Generation& gen)
	{
		while (getline(file, line) && line.find("#", 0) != 0)
		{
			if (line.empty()) continue;
			auto t = make_unique<T>();
			deserialize<Generation>(line, t);
			gen.add(std::move(t));
		}
	}

	template<class Generation>
	void deserialize(string filename, Generation& gen)
	{
		ifstream file{ filename };
		if (file)
		{
			string line;
			if (!getline(file, line)) return;

			if (line.find("# Rules", 0) == 0)
			{
				deserializeCategory<Generation::rule_t>(file, line, gen);
			}

			if (line.find("# Encoding", 0) == 0)
			{
				deserializeCategory<Generation::tile_t>(file, line, gen);
			}
		}
	}
#pragma endregion
}