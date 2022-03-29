#pragma once
#include "Hexagonal/Coord.h"
#include "Hexagonal/Math.h"
#include "ProceduralGeneration/Pattern.h"

namespace ProceduralGenerationImplementation
{
	using namespace std;

	class Tile
	{
	public:
		using Coord = Hexagonal::Coord;
		using pattern_t = ProceduralGen::Pattern<ProceduralGen::Element<char>>;
		using tag_type = std::string;
	
	private:
		Coord coords{};

		pattern_t pattern{};
	
	public:
		Tile() = default;
		Tile(Coord coords) : coords{ coords } {}

#pragma region Getters
		Coord::coord_type getR() const { return coords.r; }
		Coord::coord_type getQ() const { return coords.q; }
		Coord getCoords() const { return coords; }

		const auto& getRing() const { return pattern.data[0]; }
		const auto& getRing(size_t index) const { return pattern.data[0][index]; }

		const auto& getContraints() const { return pattern.constraints; }
		const auto& getContraints(size_t index) const { return pattern.constraints[index]; }

		float getX() const { return Hexagonal::Math::getX(getQ(), getR()); }
		float getY() const { return Hexagonal::Math::getY(getQ(), getR()); }
#pragma endregion

#pragma region Pattern
		pattern_t& getPattern()
		{
			return pattern;
		}

		const pattern_t& getPattern() const
		{
			return pattern;
		}

		void setPattern(const pattern_t& newPattern)
		{
			pattern = newPattern;
		}

		void mergePattern(const pattern_t& newPattern, const typename pattern_t::RotationInfo& rotationInfo)
		{
			pattern.merge(newPattern, rotationInfo);
		}

		void setContraintTo(const Tile& other, const typename pattern_t::element_t& element)
		{
			const size_t angle = getTileAngleTo(other);
			pattern.constraints[0][angle] = element;
		}

		void eraseConstraintTo(const Tile& other)
		{
			const size_t angle = getTileAngleTo(other);
			pattern.constraints[0][angle] = {};
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



