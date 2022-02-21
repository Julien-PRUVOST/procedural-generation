#pragma once
#include "Hexagonal/Coord.h"
#include "Hexagonal/Math.h"
#include "ProceduralGeneration/Pattern.h"

namespace ProceduralGenerationImplementation
{
	using namespace std;

	// template<class Coord>
	class Tile
	{
		using Coord = Hexagonal::Coord; // Remove for template
		using element_type = ProceduralGeneration::Pattern::element_type; // Remove for template
	
	private:
		Coord coords;

		vector<element_type> ring{};
		element_type center{};

		float angle = 0.0f;
		float size = 1.0f;

	public:

		Tile(Coord coords, float angle = 0.0f, float size = 1.0f) : coords{ coords }, angle{ angle }, size { size } {}

#pragma region Getters
		Coord::coord_type getR() const { return coords.r; }
		Coord::coord_type getQ() const { return coords.q; }
		Coord getCoords() const { return coords; }

		float getSize() const { return size; }

		float getX() const { return Hexagonal::Math::getX(getQ(), getR(), getSize()); }
		float getY() const { return Hexagonal::Math::getY(getQ(), getR(), getSize()); }
#pragma endregion

#pragma region Setters
		void setRing(vector<element_type>& patternRing)
		{
			copy(patternRing.begin(), patternRing.end(), ring.begin());
		}

		void setCenter(element_type patternCenter)
		{
			center = patternCenter;
		}
#pragma endregion

		float getAngle(const Tile &other) const
		{
			return Hexagonal::Math::getAngle(getQ(), getR(), other.getQ(), other.getR(), getSize());
		}

		void rotate() {} // Todo : add that

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



