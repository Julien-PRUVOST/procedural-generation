#pragma once
#include <cmath>

namespace ProceduralGeneration
{
	struct Math
	{
		/**
		 * \brief Angles in degrees.
		 * \param range Use range = 2*PI for radians, 360.0f for degrees
		 */
		static float getAngularDistance(float first, float second, float range = 2.0f * static_cast<float>(std::_Pi))
		{
			float raw_diff = std::fabs(first - second);
			float mod_diff = std::fmod(raw_diff, range);
			return mod_diff > range / 2.0f ? range - mod_diff : mod_diff;
		}
	};
}