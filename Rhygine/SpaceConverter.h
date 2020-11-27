#pragma once
#include "RhyMath.h"

namespace RhyM
{
	/// <summary>
	/// Gets a tangent from positions and uv coordinates.
	/// </summary>
	RhyM::Vec3 GetTangentFromTextureSpace(RhyM::Vec3 pos1, RhyM::Vec3 pos2, RhyM::Vec3 pos3, RhyM::Vec2 uv1, RhyM::Vec2 uv2, RhyM::Vec2 uv3);
}
