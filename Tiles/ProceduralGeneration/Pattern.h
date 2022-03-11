#pragma once
#include <string>
#include <vector>
#include "Element.h"
#include "../ShinMathLib/VectorMath.h"

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

		vector<element_t> constraints {};
		vector<vector<element_t>> data {};

		size_t weight{}; // TODO : Maybe should not be in the pattern

	private:
		using iterator = typename vector<element_t>::iterator;
		using const_iterator = typename vector<element_t>::const_iterator;

		static bool constrainedWhenFixed(const_iterator& begin, const const_iterator& end, const_iterator& otherBegin)
		{
			return VectorMath::compare(begin, end, otherBegin,
				[](const element_t& a, const element_t& b) {return a.constrained(b); }) == end;
		}

		static bool compatibleWhenFixed(const_iterator& begin, const const_iterator& end, const_iterator& otherBegin)
		{
			return VectorMath::compare(begin, end, otherBegin,
				[](const element_t& a, const element_t& b) {return a.compatible(b); }) == end;
		}

		template <class Predicate>
		bool testingAllRotations(const vector<element_t>& otherConstraints, const vector<vector<element_t>>& otherData, size_t& angle, Predicate f)
		{
			for (size_t iAngle = 0; iAngle != constraints.size(); ++iAngle)
			{
				if (f(constraints.begin(), constraints.end(), VectorMath::rotate(otherConstraints, iAngle).begin()))
				{
					bool found = true;
					for (size_t j = 0; j != data.size(); ++j)
					{
						if (!f(data[j].begin(), data[j].end(), VectorMath::rotate(otherData[j], iAngle).begin()))
						{
							found = false;
							break;
						}
					}

					if (found)
					{
						angle = iAngle;
						return true;
					}
				}
			}
			return false;
		}

		static bool empty(const vector<vector<element_t>>& otherData)
		{
			for (const auto& dataVector : otherData)
			{
				for (const auto& element : dataVector)
				{
					if (!element.isDefault())
						return false;
				}
			}
			return true;
		}

		bool constrained(const vector<element_t>& otherConstraints, const vector<vector<element_t>>& otherData, size_t& angle) const
		{
			return compatible(otherConstraints, otherData, angle);
			// return testingAllRotations(otherConstraints, otherData, angle, Pattern::constrainedWhenFixed);
		}

		bool compatible(const vector<element_t>& otherConstraints, const vector<vector<element_t>>& otherData, size_t& angle) const
		{
			bool result = false;
			if (empty(otherData))
			{
				result = result || testingAllRotations(otherConstraints, otherData, angle,Pattern::compatibleWhenFixed);
			}
			return result || testingAllRotations(otherConstraints, otherData, angle, Pattern::constrainedWhenFixed);
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

		void merge(const vector<element_t>& otherConstraints, const vector<vector<element_t>>& otherData, const size_t& angle)
		{
			constraints = merge(constraints.begin(), constraints.end(), VectorMath::rotate(otherConstraints, angle).begin());

			for (size_t j = 0; j!= data.size(); ++j)
			{
				data[j] = merge(data[j].begin(), data[j].end(), VectorMath::rotate(otherConstraints, angle).begin());
			}
		}

	public:
		bool constrained(const Pattern& other, size_t& angle) const
		{
			return constrained(other.constraints, other.data, angle);
		}

		/// If true, change the angle to make the rings match. \n If false, leave the angle as it was.
		bool compatible(const Pattern &other, size_t &angle) const
		{
			return constrained(other.constraints, other.data, angle);
		}

		void merge(const Pattern& other, const size_t& angle)
		{
			merge(other.constraints, other.data, angle);
		}
	};
}
