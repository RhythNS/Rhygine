#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>

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
