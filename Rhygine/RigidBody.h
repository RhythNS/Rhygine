#pragma once
#include "bullet\btBulletDynamicsCommon.h"
#include "Component.h"
#include "RhyMath.h"

#include <memory>

class Physics;
class Transform;
class RigidBody : public Component
{
	friend class Physics;
public:
	void Create(std::shared_ptr<btCollisionShape> shape, btRigidBody::btRigidBodyConstructionInfo info);
	void Create(float mass, std::shared_ptr<btCollisionShape> shape, RhyM::Vec3 localInertia = RhyM::Vec3(0.0f, 0.0f, 0.0f));
	void Create(RigidBody* copy);

	bool IsInSimulation();
	btRigidBody* GetBody();

	~RigidBody();
protected:
	void OnEnabled();
	void OnDisabled();

private:
	void UpdatePosition();

	RigidBody* prev;
	RigidBody* next;

	Transform* transform;
	std::unique_ptr<btRigidBody> body = nullptr;
	std::shared_ptr<btCollisionShape> shape = nullptr;
	btDefaultMotionState* motion = nullptr;
	bool inSimulation = false;
};
