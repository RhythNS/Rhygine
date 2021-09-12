#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>

namespace RhyBullet { class RigidBody; }

/// <summary>
/// For moving gameobjects inside a physics simulation.
/// </summary>
class TestPhysicsMover : public Component, public Updatable
{
public:
	void Init();
	void Update();
private:
	RhyBullet::RigidBody* body = nullptr;
};
