#pragma once
#include "Component.h"
#include "Updatable.h"

class RigidBody;
class TestPhysicsMover : public Component, public Updatable
{
public:
	void Init();
	void Update();
private:
	RigidBody* body;
};
