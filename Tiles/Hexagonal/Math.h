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

		static constexpr float getAngle(int q0, int r0, int q1, int r1, float size)
		{
			return atan2(getY(q1, r1, size) - getY(q0, r0, size), getX(q1, r1, size) - getX(q0, r0, size));
		}
		
	};
}
