#pragma once
#include "Component.h"
#include "Updatable.h"
#include "Transform.h"
#include "RigidBody.h"

class DeleteWhenBelow : public Component, public Updatable
{
public:
	void Init();
	void Update();
	float y = -10;

private:
	Transform* trans;
};
