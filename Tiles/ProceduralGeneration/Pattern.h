#pragma once
#include <string>
#include <vector>
#include "Element.h"

namespace ProceduralGen
{
	template<class T = Element<>>
	class Pattern
	{
	public:
		using element_t = T;
		using tag_t = std::string;

		tag_t tag {};

		vector<element_t> constraintsRing {};
		vector<element_t> dataRing{};
		element_t center{};

		size_t weight{}; // TODO : Maybe should not be in the pattern

	private:
		using iterator = typename vector<element_t>::iterator;
		using const_iterator = typename vector<element_t>::const_iterator;

		static auto rotate(const const_iterator& begin, const const_iterator& end, const size_t angle)
		{
			auto distance = std::distance(begin, end);

			std::vector<element_t> rotated;

			for (decltype(distance) i = 0;  i != distance; ++i)
			{
				rotated.push_back(*std::next(begin, (i + angle) % distance));
			}
			return rotated;
		}

		static bool compatibleWhenFixed(const const_iterator& begin, const const_iterator& end, const const_iterator& otherBegin)
		{
			auto distance = std::distance(begin, end);

			for (decltype(distance) i = 0; i != distance; ++i)
			{
				if (!(begin + i)->compatible(*(otherBegin + i))) return false;
			}
			return true;
		}

		/// If true, change the angle to make the rings match. \n If false, leave the angle as it was. 
		static bool compatibleByRotation(const const_iterator& begin, const const_iterator& end, const const_iterator& otherBegin, size_t &angle)
		{
			auto distance = std::distance(begin, end);

			for (decltype(distance) iAngle = 0; iAngle != distance; ++iAngle)
			{
				vector<element_t> firstRotated = rotate(begin, end, iAngle);
				vector<element_t> secondRotated = rotate(otherBegin, otherBegin + distance, iAngle);
				if (compatibleWhenFixed(firstRotated.begin(), firstRotated.end(), secondRotated.begin()))
				{
					angle = iAngle;
					return true;
				}
			}
			return false;
		}

		/// If true, change the angle to make the rings match. \n If false, leave the angle as it was. 
		bool compatible(const element_t& otherCenter, const vector<element_t>& otherDataRing, const vector<element_t>& otherConstraintsRing, size_t& angle) const
		{
			if (!center.compatible(otherCenter)) return false;

			for (size_t iAngle = 0; iAngle != dataRing.size(); ++iAngle)
			{
				vector<element_t> rotatedDataRing = rotate(otherDataRing.begin(), otherDataRing.end(), iAngle);
				if (compatibleWhenFixed(dataRing.begin(), dataRing.end(), rotatedDataRing.begin()))
				{
					vector<element_t> rotatedContraintRing = rotate(otherConstraintsRing.begin(), otherConstraintsRing.end(), iAngle);
					if (compatibleWhenFixed(constraintsRing.begin(), constraintsRing.end(), rotatedContraintRing.begin()))
					{
						angle = iAngle;
						return true;
					}
				}
			}

			return false;
		}

		static vector<element_t> merge(const const_iterator& begin, const const_iterator& end, const const_iterator& otherBegin)
		{
			auto distance = std::distance(begin, end);

			vector<element_t> outputRing{};

			for (decltype(distance) i = 0; i != distance; ++i)
			{
				outputRing.push_back(ProceduralGen::merge(*(begin + i), *(otherBegin + i)));
			}

			return outputRing;
		}

	public:
		/// If true, change the angle to make the rings match. \n If false, leave the angle as it was.
		bool compatible(const Pattern &other, size_t &angle) const
		{
			return compatible(other.center, other.dataRing, other.constraintsRing, angle);
		}

		void merge(const Pattern& other, const size_t& angle)
		{
			center = ProceduralGen::merge(center, other.center);
			dataRing = merge(dataRing.begin(), dataRing.end(), rotate(other.dataRing.begin(), other.dataRing.end(), angle).begin());
			constraintsRing = merge(constraintsRing.begin(), constraintsRing.end(), rotate(other.constraintsRing.begin(), other.constraintsRing.end(), angle).begin());
		}
	};
}
