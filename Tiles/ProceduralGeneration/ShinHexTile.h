#pragma once
namespace ProceduralGeneration
{
	class HexTile
	{
		using coords_type = int;
		using angle_type = int;

	public:
		coords_type q;
		coords_type r;

		angle_type hexAngle;

	public:
		HexTile(coords_type q, coords_type r, angle_type hexAngle = 0) : q{ q }, r{ r }, hexAngle{ hexAngle } {}

	};
}

