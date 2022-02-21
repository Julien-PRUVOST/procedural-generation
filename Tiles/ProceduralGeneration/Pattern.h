#pragma once
#include <string>
#include <vector>

namespace ProceduralGeneration
{
	using namespace std;

	class Pattern
	{
	public:
		using tag_type = string;
		using element_type = char;

		tag_type tag{};
		vector<element_type> constraintsRing {};
		vector<element_type> externalRing {};
		element_type center {};
	};
}
