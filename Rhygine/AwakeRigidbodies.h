#pragma once
#include "Component.h"
#include "Updatable.h"

#include <vector>

class RigidBody;
/// <summary>
/// Wakes all Rigidbodies up when the "v" button was pressed
/// </summary>
class AwakeRigidbodies : public Component, public Updatable
{
public:
	void Set(std::vector<RigidBody*> bodies);
	void Update();
private:
	std::vector<RigidBody*> bodies;
};
