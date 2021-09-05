#include <Bullet/RigidBody.h>
#include <Bullet/Physics.h>
#include <ECS/Components/Transform.h>
#include <ECS/GameObject.h>
#include <Bullet/BulletConverter.h>

void RigidBody::Init()
{
	transform = GetGameObject()->GetComponent<Transform>();
}

void RigidBody::Create(std::shared_ptr<btCollisionShape> _shape, btRigidBody::btRigidBodyConstructionInfo info)
{
	assert(_shape);
	shape = _shape;
	info.m_collisionShape = _shape.get();
	info.m_motionState = &motion;
	body = std::make_unique<btRigidBody>(info);
	Physics::Register(this);
}

void RigidBody::Create(float mass, std::shared_ptr<btCollisionShape> _shape, RhyM::Vec3 localInertia)
{
	assert(_shape);
	shape = _shape;
	body = std::make_unique<btRigidBody>(mass, &motion, shape.get(), Vec3ToBullet(localInertia));
	body->setWorldTransform(btTransform(QuatToBullet(transform->localRotation), Vec3ToBullet(transform->localPosition)));
	Physics::Register(this);
}

void RigidBody::Create(RigidBody* copy)
{
	assert(copy);
	shape = copy->shape;
	body = std::make_unique<btRigidBody>(copy->body->getMass(), &motion, shape.get(), copy->body->getLocalInertia());
	body->setWorldTransform(btTransform(QuatToBullet(transform->localRotation), Vec3ToBullet(transform->localPosition)));
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

btDefaultMotionState* RigidBody::GetMotionState()
{
	return &motion;
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
	transform->localPosition = Vec3ToRhy(body->getWorldTransform().getOrigin());
	transform->localRotation = QuatToRhy(body->getOrientation());
}
