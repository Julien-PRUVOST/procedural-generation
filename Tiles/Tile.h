#pragma once
#include "Hexagonal/Coord.h"
#include "Hexagonal/Math.h"
#include "ProceduralGeneration/Pattern.h"

namespace ProceduralGenerationImplementation
{
	using namespace std;

	// template<class Coord>
	template <class Pattern>
	class Tile
	{
	public:
		using Coord = Hexagonal::Coord; // Remove for template
		using pattern_t = Pattern;
		using tag_type = std::string;
	
	private:
		Coord coords{};

		pattern_t pattern {};

		vector<tag_type> tags{};
	
	public:
		Tile() = default;
		Tile(Coord coords) : coords{ coords } {}

#pragma region Getters
		Coord::coord_type getR() const { return coords.r; }
		Coord::coord_type getQ() const { return coords.q; }
		Coord getCoords() const { return coords; }

		const vector<tag_type>& getTags() const { return tags; }

		const auto& getRing() const { return pattern.data[0]; }
		const auto& getRing(size_t index) const { return pattern.data[0][index]; }

		const auto& getContraints() const { return pattern.constraints; }
		const auto& getContraints(size_t index) const { return pattern.constraints[index]; }

		float getX() const { return Hexagonal::Math::getX(getQ(), getR()); }
		float getY() const { return Hexagonal::Math::getY(getQ(), getR()); }
#pragma endregion

#pragma region Pattern
		const pattern_t& getPattern() const
		{
			return pattern;
		}

		void setPattern(pattern_t newPattern)
		{
			pattern = newPattern;

			tags.clear();
			tags.push_back(newPattern.tag);
		}

		vector<size_t> getCompatibleAngles(const pattern_t &newPattern) const
		{
			return pattern.getCompatibleAngles(newPattern);
		}

		void mergePattern(const pattern_t& newPattern, const size_t& angle)
		{
			pattern.merge(newPattern, angle);

			if (std::find(tags.begin(), tags.end(), newPattern.tag) == tags.end())
				tags.push_back(newPattern.tag);
		}

		void setContraintTo(const Tile& other, typename pattern_t::element_t element)
		{
			const size_t angle = getTileAngleTo(other);
			pattern.constraints[0][angle] = element;
		}
#pragma endregion

		int distance(const Tile& other) const
		{
			return Hexagonal::Math::distance(getQ(), getR(), other.getQ(), other.getR());
		}

		size_t getTileAngleTo(const Tile &other) const
		{
			return Hexagonal::Math::getHexAngle(getQ(), getR(), other.getQ(), other.getR());
		}

		float getAngleDegreesTo(const Tile& other) const
		{
			return Hexagonal::Math::getAngleDegrees(getQ(), getR(), other.getQ(), other.getR());
		}

		void erase()
		{
			setPattern({ {}, {{{}, {} ,{}, {}, {}, {}}}, {{{}, {} ,{}, {}, {}, {}}, {{}}}, {} });
			tags.clear();
		}

		static float getAngleDegrees(int hexAngle)
		{
			return Hexagonal::Math::getAngleDegrees(hexAngle);
		}

		bool operator==(const Tile &other) const
		{
			return getCoords() == other.getCoords();
		}

		bool operator!=(const Tile& other) const
		{
			return !(*this == other);
		}
	};
	
}



