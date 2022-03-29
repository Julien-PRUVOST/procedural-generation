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

		vector<tag_t> tags {};

		vector<vector<element_t>> constraints {};
		vector<vector<element_t>> data {};

	private:
		using iterator = typename vector<element_t>::iterator;
		using const_iterator = typename vector<element_t>::const_iterator;

		static bool _constraintCanReceive(const_iterator begin, const const_iterator end, const_iterator otherBegin)
		{
			return VectorMath::compare(begin, end, otherBegin, &ProceduralGen::constraintCanReceive<typename element_t::value_t>) == end;
		}

		static bool _canReceive(const_iterator begin, const const_iterator end, const_iterator otherBegin)
		{
			return VectorMath::compare(begin, end, otherBegin, &ProceduralGen::canReceive<typename element_t::value_t>) == end;
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

		vector<size_t> testRotations(const Pattern& other) const
		{
			const vector<size_t> anglesConstrained = testAllRotations(constraints, other.constraints, &Pattern::_constraintCanReceive);

			vector<size_t> anglesConstrainedAndCompatible;
			for (const size_t& angleConstrained : anglesConstrained)
			{
				if (testRotation(data, other.data, angleConstrained, &Pattern::_canReceive))
					anglesConstrainedAndCompatible.push_back(angleConstrained);
			}

			return anglesConstrainedAndCompatible;
		}

		static vector<element_t> flip(vector<element_t> ring)
		{
			return VectorMath::reverse_inplace(ring);
		}

		static vector<vector<element_t>> flip(const vector<vector<element_t>>& informations)
		{
			vector<vector<element_t>> flippedInformations;

			for (const auto& ring : informations)
			{
				flippedInformations.push_back(flip(ring));
			}

			return flippedInformations;
		}

		Pattern flip() const
		{
			auto otherConstraints = flip(constraints);
			auto otherData = flip(data);

			return{tags,
					otherConstraints,
					otherData};
		}


		void reset(vector<vector<element_t>>& v)
		{
			for (size_t i = 0; i != v.size(); ++i)
			{
				for (size_t j = 0; j != v[i].size(); ++j)
				{
					v[i][j] = element_t{};
				}
			}
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
		struct RotationInfo
		{
			size_t angle;
			bool flipped;
		};

	private:
		void mergeData(vector<vector<element_t>> otherData, const RotationInfo& rotationInfo)
		{
			if (rotationInfo.flipped)
				otherData = flip(otherData);

			for (size_t i = 0; i != data.size(); ++i)
			{
				data[i] = merge(data[i].begin(), data[i].end(), VectorMath::rotate(otherData[i], rotationInfo.angle).begin());
			}
		}

	public:
		vector<RotationInfo> canReceive(const Pattern& other) const
		{
			vector<RotationInfo> result;

			const vector<size_t> angles = testRotations(other);

			for (const size_t& angle : angles)
			{
				result.push_back({ angle, false });
			}

			const vector<size_t> anglesOfFlippedPatterns = testRotations(other.flip());

			for (const size_t& angleOfFlippedPattern : anglesOfFlippedPatterns)
			{
				result.push_back({ angleOfFlippedPattern, true });
			}

			return result;
		}

		void merge(const Pattern& other, const RotationInfo& rotationInfo)
		{
			resetConstraints();
			mergeData(other.data, rotationInfo);

			for (const tag_t& tag : other.tags)
			{
				if (std::find(tags.begin(), tags.end(), tag) == tags.end()) tags.push_back(tag);
			}
		}

		void resetData()
		{
			reset(data);
		}

		void resetConstraints()
		{
			reset(constraints);
		}

		void reset()
		{
			resetData();
			resetConstraints();
		}

		void reset(vector<size_t> constraintsNb, vector<size_t> dataNb)
		{
			tags = {};

			for (size_t i : constraintsNb)
				constraints.push_back(vector<element_t>(i, element_t{}));

			for (size_t i : dataNb)
				data.push_back(vector<element_t>(i, element_t{}));
		}

	};
}
