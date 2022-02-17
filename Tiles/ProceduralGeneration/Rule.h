#pragma once
#include <sstream>
#include <string>

namespace ProceduralGeneration
{
	using namespace std;

	class Rule
	{
	public:
		using tile_element_type = char;
		using value_type = unsigned int;

	private:
		tile_element_type tileElement0 = '0';
		tile_element_type tileElement1 = '0';
		tile_element_type newTileElement = '0';

		value_type n = 0;

	private:
		enum class RuleType
		{
			None,
			MinLength,
			MaxConflict,
			ConflitTransform
		};

		RuleType ruleType = RuleType::None;

	public:
		void create(tile_element_type tileElement, value_type minLength)
		{
			ruleType = RuleType::MinLength;

			tileElement0 = tileElement;
			n = minLength;
		}

		void create(tile_element_type tileElement, tile_element_type otherTileElement, value_type maxConflictOnTile)
		{
			ruleType = RuleType::MaxConflict;

			tileElement0 = tileElement;
			tileElement1 = otherTileElement;
			n = maxConflictOnTile;
		}

		void create(tile_element_type tileElement0_, tile_element_type tileElement1_, tile_element_type newTileElement_)
		{
			ruleType = RuleType::ConflitTransform;

			tileElement0 = tileElement0_;
			tileElement1 = tileElement1_;
			newTileElement = newTileElement_;
		}
	};


#pragma region Deserialization

	inline bool decodeAsInt(const string &word, char &c, unsigned int &n)
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

	inline void deserialize(const string &line, unique_ptr<Rule>& rule)
	{
		istringstream input {line};

		string subLine;
		bool mask[3] = {true, true, true};
		Rule::tile_element_type params[3];
		Rule::value_type n = 0;

		for (int i = 0; i < 3 && getline(input, subLine, ' '); ++i)
		{
			mask[i] = decodeAsInt(subLine, params[i], n);
		}

		if (!mask[0] && mask[1] && mask[2]) rule->create(params[0], n);
		if (!mask[0] && !mask[1] && mask[2]) rule->create(params[0], params[1], n);
		if (!mask[0] && !mask[1] && !mask[2]) rule->create(params[0], params[1], params[2]);
	}

#pragma endregion
}


