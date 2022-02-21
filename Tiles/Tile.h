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

		int tileAngle = 0;
		float size = 1.0f;

	public:

		Tile(Coord coords, int tileAngle = 0, float size = 1.0f) : coords{ coords }, tileAngle{ tileAngle }, size { size }
		{}

#pragma region Getters
		Coord::coord_type getR() const { return coords.r; }
		Coord::coord_type getQ() const { return coords.q; }
		Coord getCoords() const { return coords; }

		float getSize() const { return size; }

		float getX() const { return Hexagonal::Math::getX(getQ(), getR(), getSize()); }
		float getY() const { return Hexagonal::Math::getY(getQ(), getR(), getSize()); }
		float getAngleDegrees() const { return Hexagonal::Math::getAngleDegrees(tileAngle); }
#pragma endregion

#pragma region Setters
		void setRing(const vector<element_type>& patternRing)
		{
			ring.reserve(6);
			copy(patternRing.begin(), patternRing.end(),  back_inserter(ring));
		}

		void addToRing(const vector<element_type>& patternRing)
		{
			if (ring.empty()) setRing(patternRing);
			else
			{
				for (size_t i = 0; i != patternRing.size(); ++i)
				{
					if (patternRing[i] != element_type{} && ring[i] == element_type{})
						ring[i] = patternRing[i];
				}
			}
		}

		void setCenter(element_type patternCenter)
		{
			center = patternCenter;
		}

		class bad_angle_exception : std::exception{};

		/// Angle between 0 and 5
		void setTileAngle(int newAngle)
		{
			if (0 <= newAngle && newAngle < 6) tileAngle = newAngle;
			else throw bad_angle_exception{};
		}
#pragma endregion

		int getTileAngleTo(const Tile &other) const
		{
			return Hexagonal::Math::getHexAngle(getQ(), getR(), other.getQ(), other.getR());
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



