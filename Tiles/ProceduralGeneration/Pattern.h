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

		vector<vector<element_t>> constraints {};
		vector<vector<element_t>> data {};

		size_t weight{}; // TODO : Maybe should not be in the pattern

	private:
		using iterator = typename vector<element_t>::iterator;
		using const_iterator = typename vector<element_t>::const_iterator;

		static bool constraining(const_iterator begin, const const_iterator end, const_iterator otherBegin)
		{
			return VectorMath::compare(begin, end, otherBegin, &ProceduralGen::constraining<element_t>) == end;
		}

		static bool compatible(const_iterator begin, const const_iterator end, const_iterator otherBegin)
		{
			return VectorMath::compare(begin, end, otherBegin, &ProceduralGen::compatible<element_t>) == end;
		}

		template <class Predicate>
		static bool testRotation(const vector<vector<element_t>>& v0, const vector<vector<element_t>>& v1, const size_t& angle, Predicate pred)
		{
			return VectorMath::compare(v0.begin(), v0.end(), v1.begin(), [&pred, &angle](const vector<element_t>& ring0, const vector<element_t>& ring1)
			{
				return pred(ring0.begin(), ring0.end(), VectorMath::rotate(ring1, angle).begin());
			}) == v0.end();
		}

		template <class Predicate>
		static vector<size_t> testAllRotations(const vector<vector<element_t>>& v0, const vector<vector<element_t>>& v1, Predicate pred)
		{
			vector<size_t> angles;
			// Todo : We detetct the number of rotations to perform from v0[0]. We should not
			for (size_t angle = 0; angle != v0[0].size(); ++angle)
			{
				if (testRotation(v0, v1, angle, pred))
					angles.push_back(angle);
			}
			return angles;
		}

		vector<size_t> getCompatibleAngles(const vector<vector<element_t>>& otherConstraints, const vector<vector<element_t>>& otherData) const
		{
			vector<size_t> anglesConstrained;
			for (size_t i = 0; i != constraints.size(); ++i)
			{
				anglesConstrained = testAllRotations(constraints, otherConstraints, &Pattern::constraining);
			}

			vector<size_t> anglesConstrainedAndCompatible;
			for (const size_t& angleConstrained : anglesConstrained)
			{
				if (testRotation(data, otherData, angleConstrained, &Pattern::compatible))
					anglesConstrainedAndCompatible.push_back(angleConstrained);
			}

			return anglesConstrainedAndCompatible;
		}


		static vector<element_t> merge(const const_iterator& begin, const const_iterator& end, const const_iterator& otherBegin)
		{
			auto distance = std::distance(begin, end);

			vector<element_t> outputRing{};

			for (size_t i = 0; i != distance; ++i)
			{
				outputRing.push_back(ProceduralGen::merge(*(begin + i), *(otherBegin + i)));
			}

			return outputRing;
		}

		void merge(const vector<vector<element_t>>& otherConstraints, const vector<vector<element_t>>& otherData, const size_t& angle)
		{

			for (size_t i = 0; i != constraints.size(); ++i)
			{
				for (size_t j = 0; j != constraints[i].size(); ++j)
				{
					constraints[i][j] = element_t{};
				}
			}

			for (size_t i = 0; i!= data.size(); ++i)
			{
				data[i] = merge(data[i].begin(), data[i].end(), VectorMath::rotate(otherData[i], angle).begin());
			}
		}

	public:
		vector<size_t> getCompatibleAngles(const Pattern &other) const
		{
			return getCompatibleAngles(other.constraints, other.data);
		}

		void merge(const Pattern& other, const size_t& angle)
		{
			merge(other.constraints, other.data, angle);
		}
	};
}
