#pragma once
#include "RhyMath.h"
#include "RhyWin.h"

#include <DirectXMath.h>

/// <summary>
/// Representation of a 2D camera.
/// </summary>
class OrthographicCamera
{
public:
	OrthographicCamera(
		RhyM::Vec3 position = RhyM::Vec3(0.0f, 0.0f, -10.0f),
		RhyM::Vec2 rotation = RhyM::Vec2(0.0f, 0.0f),
		RhyM::Vec2 zoom = RhyM::Vec2(1.0f, 1.0f));

	RhyM::Vec3 position;
	RhyM::Vec2 rotation;
	RhyM::Vec2 zoom;
	/// <summary>
	/// Gets the transposed orthographic matrix.
	/// </summary>
	DirectX::XMMATRIX GetOrthoMatrix();
};
