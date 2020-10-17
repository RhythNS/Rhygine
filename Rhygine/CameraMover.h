#pragma once
#include "Component.h"
#include "Updatable.h"

class Transform;

class CameraMover : public Component, public Updatable
{
public:
	void Init();
	void Update();
private:
	Transform* transform;
};
