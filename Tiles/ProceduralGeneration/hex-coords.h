#pragma once

class HexCoords
{
public:
	static constexpr float SQRT3 = 1.73205080757f;

	/// Used in loop, to convert row / column coords to hex coords
	static int getR(int i, int j) noexcept { return i; }

	/// Used in loop, to convert row / column coords to hex coords
	static int getQ(int i, int j) noexcept { return j - i / 2; }

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
	static constexpr float getY(int r, float size)
	{
		const float h = 2.0f * size;
		return static_cast<float>(r) * 0.75f * h;
	}

	/**
	 * @param size diameter of the tile
	 * @return Y coordinate of the center of the Tile at (q, r)
	 */
	static constexpr float getY(int q, int r, float size) {
		return getY(r, size);
	}


	/**
	 * Remaps 0-5 to 0-360
	 *
	 * @param hexAngle Hexagonal Angle, between 0 and 5
	 * @return Angle in degrees
	 */
	static int getAngle(int hexAngle) noexcept { return hexAngle * 60; }
};
