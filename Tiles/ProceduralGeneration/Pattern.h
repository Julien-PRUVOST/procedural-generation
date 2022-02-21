#pragma once
#include <string>
#include <vector>

namespace ProceduralGeneration
{
	class Pattern
	{
	public:
		using tag_type = std::string;
		using element_type = char;

		tag_type tag{};
		std::vector<element_type> constraintsRing {};
		std::vector<element_type> externalRing {};
		element_type center {};

		static std::vector<element_type> rotateRing(const std::vector<element_type> &ring, int n)
		{
			std::vector<element_type> rotatedRing;

			for (size_t i = 0; i != ring.size(); ++i)
			{
				rotatedRing.push_back(ring[(i + n) % ring.size()]);
			}

			return rotatedRing;
		}
	};
}
