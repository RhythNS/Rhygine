#pragma once
#include "Component.h"
#include "Updatable.h"

#include <vector>

class RigidBody;
class AwakeRigidbodies : public Component, public Updatable
{
public:
	void Set(std::vector<RigidBody*> bodies);
	void Update();
private:
	std::vector<RigidBody*> bodies;
};
