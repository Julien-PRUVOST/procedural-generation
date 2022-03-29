#pragma once

namespace Hexagonal
{
	struct Coord
	{
		using coord_type = int;

		coord_type q;
		coord_type r;

		Coord() = default;
		Coord(coord_type q, coord_type r) : q{ q }, r{ r } {}

#pragma region Static
		/// Used in loop, to convert row / column coords to hex coords
		static coord_type getQ(size_t i, size_t j) noexcept { return static_cast<coord_type>(j) - getR(i, j) / 2; }

		/// Used in loop, to convert row / column coords to hex coords
		static coord_type getR(size_t i, size_t j) noexcept { return static_cast<coord_type>(i); }

		static Coord getCoord(size_t i, size_t j) {	return {getQ(i, j), getR(i, j)}; }
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
