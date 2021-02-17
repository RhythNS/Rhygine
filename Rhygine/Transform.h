#pragma once
#include "RhyMath.h"
#include "Component.h"

#include <DirectXMath.h>
#include <vector>

/// <summary>
/// Holds the position, scale and rotation of a gameobject. This component is needed
/// when drawing it onto the screen.
/// </summary>
class Transform : public Component
{
public:
	~Transform();

	/// <summary>
	/// Sets the parent of this transform.
	/// </summary>
	void SetParent(Transform* trans);
	/// <summary>
	/// Gets a pointer to the parent of the transform.
	/// </summary>
	Transform* GetParent();
	/// <summary>
	/// Gets the child transform at given index.
	/// </summary>
	Transform* GetChildAt(int index);
	/// <summary>
	/// Gets the total amount of children this transform has.
	/// </summary>
	int GetChildCount();

	/// <summary>
	/// Gets the rotated forward vector of the gameobject.
	/// </summary>
	RhyM::Vec3 GetForward();
	/// <summary>
	/// Gets the rotated backward vector of the gameobject.
	/// </summary>
	RhyM::Vec3 GetBackwards();
	/// <summary>
	/// Gets the rotated left vector of the gameobject.
	/// </summary>
	RhyM::Vec3 GetLeft();
	/// <summary>
	/// Gets the rotated right vector of the gameobject.
	/// </summary>
	RhyM::Vec3 GetRight();
	/// <summary>
	/// Gets the rotated up vector of the gameobject.
	/// </summary>
	RhyM::Vec3 GetUp();
	/// <summary>
	/// Gets the rotated down vector of the gameobject.
	/// </summary>
	RhyM::Vec3 GetDown();

	DirectX::XMMATRIX GetWorldMatrix();
	DirectX::XMMATRIX GetPerspectiveMatrix();
	DirectX::XMMATRIX GetLocalMatrix();

	void SetWorldPosition(RhyM::Vec3 pos);
	RhyM::Vec3 GetWorldPosition();
	RhyM::Vec3 localPosition = RhyM::Vec3(0.0f, 0.0f, 0.0f);

	void SetWorldRotation(RhyM::Quat rotation);
	RhyM::Quat GetWorldRotation();
	RhyM::Quat localRotation = RhyM::Quat::FromEuler(0.0f, 0.0f, 0.0f);

	void SetWorldScale(RhyM::Vec3 scale);
	RhyM::Vec3 GetWorldScale();
	RhyM::Vec3 localScale = RhyM::Vec3(1.0f, 1.0f, 1.0f);
private:
	Transform* parent = nullptr;
	std::vector<Transform*>children{};
};
