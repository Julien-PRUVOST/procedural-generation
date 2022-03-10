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
		using tag_t = std::string;
		using element_t = T;

		using ring_t = std::vector<element_t>;

		tag_t tag {};
		ring_t constraintsRing {};
		ring_t externalRing {};
		element_t center {};
		size_t weight{}; // TODO : Maybe should not be in the pattern

	private:
		static ring_t rotateRing(const ring_t& ring, const size_t angle)
		{
			std::vector<element_t> rotatedRing;

			for (size_t i = 0; i != ring.size(); ++i)
			{
				rotatedRing.push_back(ring[(i + angle) % ring.size()]);
			}

			return rotatedRing;
		}

		/// Assuming both rings have the same type and comparable elements
		static bool compatibleWhenFixed(const ring_t &ring, const ring_t& otherRing)
		{
			for (size_t i = 0; i != ring.size(); ++i)
			{
				if (!ring[i].compatible(otherRing[i])) return false;
			}
			return true;
		}

		/// If true, change the angle to make the rings match. \n If false, leave the angle as it was. 
		static bool compatibleByRotation(const ring_t& ring, const ring_t& otherRing, size_t &angle)
		{
			for (size_t iAngle = 0; iAngle != ring.size(); ++iAngle)
			{
				if (compatibleWhenFixed(rotateRing(ring, iAngle), rotateRing(otherRing, iAngle)))
				{
					angle = iAngle;
					return true;
				}
			}
			return false;
		}

		/// If true, change the angle to make the rings match. \n If false, leave the angle as it was. 
		bool compatible(const element_t& otherCenter, const ring_t& otherExternalRing, const ring_t& otherConstraintRing, size_t& angle) const
		{
			if (!center.compatible(otherCenter)) return false;

			for (size_t iAngle = 0; iAngle != otherExternalRing.size(); ++iAngle)
			{
				if (compatibleWhenFixed(externalRing, rotateRing(otherExternalRing, iAngle)))
				{
					if (compatibleWhenFixed(constraintsRing, rotateRing(otherConstraintRing, iAngle)))
					{
						angle = iAngle;
						return true;
					}
				}
			}

			return false;
		}

		static ring_t merge(const ring_t &ring, const ring_t &otherRing)
		{
			ring_t outputRing{};

			for (size_t i = 0; i != ring.size(); ++i)
			{
				outputRing.push_back(ProceduralGen::merge(ring[i], otherRing[i]));
			}

			return outputRing;
		}

	public:
		/// If true, change the angle to make the rings match. \n If false, leave the angle as it was.
		bool compatible(const Pattern &other, size_t &angle) const
		{
			return compatible(other.center, other.externalRing, other.constraintsRing, angle);
		}

		void merge(const Pattern& other, const size_t& angle)
		{
			center = ProceduralGen::merge(center, other.center);
			externalRing = merge(externalRing, rotateRing(other.externalRing, angle));
			constraintsRing = merge(constraintsRing, rotateRing(other.constraintsRing, angle));
		}
	};
}
