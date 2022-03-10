#pragma once
#include <cmath>


namespace shinmathlib
{
	namespace Math
	{
		constexpr double _PI = 3.141592653589793238462643383279502884L;

		/// Use range = 2*PI for radians, 360.0f for degrees
		inline float angularDifference(const float first, const float second, float range = 360.0f)
		{
			const float diff = first - second;
			if (diff < -range / 2) return diff + range;
			if (range / 2 < diff) return diff - range;
			return diff;
		}

		/// Use range = 2*PI for radians, 360.0f for degrees
		inline float getSmallestAbsoluteAngularDistance(float first, float second, float range = 2.0f * static_cast<float>(_PI))
		{
			const float raw_diff = std::fabs(first - second);
			const float mod_diff = std::fmod(raw_diff, range);
			return mod_diff > range / 2.0f ? range - mod_diff : mod_diff;
		}
	};
};

