#pragma once
// Includer for including math classes.

#include "assimp\vector2.h"
#include "assimp\vector3.h"
#include "assimp\matrix3x3.h"
#include "assimp\matrix4x4.h"
#include "assimp\quaternion.h"

// I am not sure how to force include these in vcpkg. If this is not done then the implentations
// are not loaded during compilation. Doing in this way is probably increasing compile time by quite
// a bit.
#include "assimp\vector2.inl"
#include "assimp\vector3.inl"
#include "assimp\matrix3x3.inl"
#include "assimp\matrix4x4.inl"
#include "assimp\quaternion.inl"

namespace RhyM {
	typedef aiVector2t<float> Vec2;
	typedef aiVector2t<int> Vec2I;
	typedef aiVector3t<float> Vec3;
	typedef aiVector3t<int> Vec3I;
	typedef aiQuaterniont<float> Quat;
	typedef aiMatrix3x3t<float> Mat3;
	typedef aiMatrix4x4t<float> Mat4;

	constexpr float PI = 3.14159265358979323846f;
}
