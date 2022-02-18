#pragma once
#include <sstream>
#include <string>
#include <memory>

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
}


