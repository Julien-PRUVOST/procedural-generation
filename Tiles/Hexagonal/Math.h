#pragma once

namespace Hexagonal
{
	struct Math
	{
		static constexpr float SQRT3 = 1.73205080757f;

		/**
		 * @param size diameter of the Tile
		 * @return X coordinate of the center of the Tile at (q, r)
		 */
		static constexpr float getX(int q, int r, float size)
		{
			const float w = SQRT3 * size;
			return static_cast<float>(q) * w + static_cast<float>(r) * 0.5f * w;
		}

		/**
		 * @param size diameter of the Tile
		 * @return Y coordinate of the center of the Tile at (q, r)
		 */
		static constexpr float getY(int q, int r, float size)
		{
			const float h = 2.0f * size;
			return static_cast<float>(r) * 0.75f * h;
		}

		static int getHexAngle(int q0, int r0, int q1, int r1)
		{
			float radAngle = atan2(getY(q1, r1, 1.0f) - getY(q0, r0, 1.0f), getX(q1, r1, 1.0f) - getX(q0, r0, 1.0f));
			int conversion = std::lround(radAngle / (static_cast<float>(std::_Pi) / 3.0f));
			return (conversion + 6) % 6;
		}

		static constexpr float getAngleDegrees(int hexAngle)
		{
			return static_cast<float>(hexAngle) * 60.0f;
		}

		static float getAngleDegrees(int q0, int r0, int q1, int r1)
		{
			float radAngle = atan2(getY(q1, r1, 1.0f) - getY(q0, r0, 1.0f), getX(q1, r1, 1.0f) - getX(q0, r0, 1.0f));
			return radAngle / (2 * static_cast<float>(std::_Pi)) * 360.0f;
		}
		
	};
}
