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

	/// <summary>
	/// Gets the current matrix which gets updated at the start of a
	/// draw call from the stage.
	/// </summary>
	DirectX::XMMATRIX* GetMatrix();
private:
	/// <summary>
	/// Updates the matrix.
	/// </summary>
	void UpdateMatrix();

	Transform* transform = nullptr;
	DirectX::XMMATRIX matrix{};
};
