#pragma once
#include "Component.h"
#include "Updatable.h"
#include "RhyMath.h"

class Transform;

class RotateAround : public Component, public Updatable
{
public:
	void Init();
	void Update();

	Vec3 rotationSpeed = Vec3(1.0f, 1.0f, 1.0f);
private:
	Transform* transform;
};

