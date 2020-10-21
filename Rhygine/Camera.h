#pragma once
#include "Component.h"

#include <DirectXMath.h>

class Transform;
class Stage;

/// <summary>
/// Holds a matrix to draw gameobjects to the screen.
/// </summary>
class Camera : public Component
{
	friend class Stage;
public:
	void Init();
	Transform* GetTransform();

	DirectX::XMMATRIX* GetMatrix();
private:
	/// <summary>
	/// Updates the matrix.
	/// </summary>
	void UpdateMatrix();

	Transform* transform;
	DirectX::XMMATRIX matrix;
};
