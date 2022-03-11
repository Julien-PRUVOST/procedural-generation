#pragma once
#include <string>
#include <vector>
#include "ShinMathLib/VectorMath.h"
#include "Element.h"

namespace ProceduralGen
{
	using namespace shinmathlib;

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

		static bool compatibleWhenFixed(const const_iterator& begin, const const_iterator& end, const const_iterator& otherBegin)
		{
			return VectorMath::compare(begin, end, otherBegin,
				[](const element_t& a, const element_t& b) {return a.compatible(b); }) == end;
		}

		/// If true, change the angle to make the rings match. \n If false, leave the angle as it was. 
		bool compatible(const element_t& otherCenter, const vector<element_t>& otherDataRing, const vector<element_t>& otherConstraintsRing, size_t& angle) const
		{
			if (!center.compatible(otherCenter)) return false;

			for (size_t iAngle = 0; iAngle != dataRing.size(); ++iAngle)
			{
				if (compatibleWhenFixed(dataRing.begin(), dataRing.end(), VectorMath::rotate(otherDataRing, iAngle).begin()))
				{
					if (compatibleWhenFixed(constraintsRing.begin(), constraintsRing.end(), VectorMath::rotate(otherConstraintsRing, iAngle).begin()))
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
			dataRing = merge(dataRing.begin(), dataRing.end(), VectorMath::rotate(other.dataRing, angle).begin());
			constraintsRing = merge(constraintsRing.begin(), constraintsRing.end(), VectorMath::rotate(other.constraintsRing, angle).begin());
		}
	};
}
