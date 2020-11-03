#pragma once
#include "RhyMath.h"
#include "Component.h"

#include <DirectXMath.h>

/// <summary>
/// Holds the position, scale and rotation of a gameobject. This component is needed
/// when drawing it onto the screen.
/// </summary>
class Transform : public Component
{
public:
	void Init();
	RhyM::Vec3 position = RhyM::Vec3(0.0f,0.0f,0.0f);
	RhyM::Quat rotation = RhyM::Quat(0.0f, 0.0f, 0.0f);
	RhyM::Vec3 scale = RhyM::Vec3(1.0f,1.0f,1.0f);

	/// <summary>
	/// Gets the rotated forward vector of the gameobject.
	/// </summary>
	RhyM::Vec3 GetForward();

	DirectX::XMMATRIX* GetWorldMatrix();
	DirectX::XMMATRIX* GetPerspectiveMatrix();
	DirectX::XMMATRIX* GetLocalMatrix();
private:
	DirectX::XMMATRIX perspectiveMatrix, worldMatrix, localMatrix;
};
