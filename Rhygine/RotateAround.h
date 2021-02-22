#pragma once
#include "Component.h"
#include "Updatable.h"
#include "RhyMath.h"

class Transform;

/// <summary>
/// Rotates the transform with a rotationspeed.
/// </summary>
class RotateAround : public Component, public Updatable
{
public:
	void Init();
	void Update();

	/// <summary>
	/// Rotates the gameobject by this amount per second.
	/// </summary>
	RhyM::Vec3 rotationSpeed = RhyM::Vec3(1.0f, 1.0f, 1.0f);
private:
	Transform* transform = nullptr;
};

