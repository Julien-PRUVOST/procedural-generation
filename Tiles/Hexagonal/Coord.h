#pragma once

namespace Hexagonal
{
	struct Coord
	{
		using coord_type = int;
		

		coord_type q;
		coord_type r;

		Coord(coord_type q, coord_type r) : q{ q }, r{ r } {}

#pragma region Static
		/// Used in loop, to convert row / column coords to hex coords
		static coord_type getQ(int i, int j) noexcept { return j - i / 2; }

		/// Used in loop, to convert row / column coords to hex coords
		static coord_type getR(int i, int j) noexcept { return i; }

		static Coord getCoord(int i, int j)
		{
			return {getQ(i, j), getR(i, j)};
		}
#pragma endregion

		bool operator==(const Coord &other) const
		{
			return q == other.q && r == other.r;
		}

		bool operator!=(const Coord &other) const
		{
			return !(*this == other);
		}

	};
}