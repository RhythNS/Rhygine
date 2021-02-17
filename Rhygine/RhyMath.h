#pragma once
// Includer for including math classes.

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector2Int.hpp"
#include "Quaternion.hpp"
#include "Matrix3x3.hpp"

namespace RhyM {
	typedef Vector2 Vec2;
	typedef Vector2Int Vec2I;
	typedef Vector3 Vec3;
	typedef Quaternion Quat;
	typedef Matrix3x3 Mat3;

	constexpr float PI = 3.14159265358979323846f;
}
