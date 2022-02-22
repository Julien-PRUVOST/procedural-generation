#pragma once
#include <cmath>

namespace ProceduralGeneration
{
	struct Math
	{
		/**
		 * \param range Use range = 2*PI for radians, 360.0f for degrees
		 */
		static float getAngularDifference(float first, float second, float range = 360.0f)
		{
			float diff = first - second;
			if (diff < -range / 2) return diff + range;
			if (range / 2 < diff) return diff - range;
			return diff;
		}

		/**
		 * \param range Use range = 2*PI for radians, 360.0f for degrees
		 */
		static float getSmallestAbsoluteAngularDistance(float first, float second, float range = 2.0f * static_cast<float>(std::_Pi))
		{
			float raw_diff = std::fabs(first - second);
			float mod_diff = std::fmod(raw_diff, range);
			return mod_diff > range / 2.0f ? range - mod_diff : mod_diff;
		}
	};
}