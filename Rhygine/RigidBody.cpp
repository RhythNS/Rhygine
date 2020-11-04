#include "RigidBody.h"
#include "Physics.h"
#include "Transform.h"

void RigidBody::Create(std::shared_ptr<btCollisionShape> _shape, btRigidBody::btRigidBodyConstructionInfo info)
{
	shape = _shape;
	info.m_collisionShape = _shape.get();
	body = std::make_unique<btRigidBody>(info);
	Physics::Register(this);
}

void RigidBody::Create(float mass, std::shared_ptr<btCollisionShape> _shape, RhyM::Vec3 localInertia)
{
	shape = _shape;
	body = std::make_unique<btRigidBody>(mass, motion, shape.get(), localInertia);
	body->translate(transform->position);
	Physics::Register(this);
}

void RigidBody::Create(RigidBody* copy)
{
	shape = copy->shape;
	body = std::make_unique<btRigidBody>(*copy->body);
	body->translate(transform->position);
	Physics::Register(this);
}

bool RigidBody::IsInSimulation()
{
	return inSimulation;
}

btRigidBody* RigidBody::GetBody()
{
	return body.get();
}

RigidBody::~RigidBody()
{
	if (inSimulation)
		Physics::DeRegister(this);
}

void RigidBody::OnEnabled()
{
	if (!inSimulation)
		Physics::Register(this);
}

void RigidBody::OnDisabled()
{
	if (inSimulation)
		Physics::DeRegister(this);
}

void RigidBody::UpdatePosition()
{
	transform->position = body->getWorldTransform().getOrigin();
	transform->rotation = body->getOrientation();
}
