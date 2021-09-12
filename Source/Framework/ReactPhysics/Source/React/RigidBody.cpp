#include <React/RigidBody.h>
#include <React/ReactConverter.h>
#include <React/ReactPhysicsModule.h>

#include <reactphysics3d/engine/PhysicsWorld.h>

#include <ECS/Components/Transform.h>
#include <ECS/GameObject.h>

using namespace RhyReact;

RigidBody::~RigidBody()
{
	if (!body)
		return;

	ReactBodyBase::~ReactBodyBase();
	GetWorld()->destroyRigidBody(body);
	ReactPhysicsModule::DeRegister(this);
}

void RigidBody::Init()
{
	transform = GetGameObject()->GetComponent<Transform>();
	assert(transform);

	collisionBody = body = GetWorld()->createRigidBody(
		TransformRhyToReact(transform->GetWorldPosition(), transform->GetWorldRotation()));
	body->setUserData(this);

	ReactPhysicsModule::Register(this);
}

float RigidBody::GetMass()
{
	assert(body);
	return body->getMass();
}

void RigidBody::SetMass(float mass)
{
	assert(body);
	body->setMass(mass);
}

RhyM::Vec3 RigidBody::GetLinearVelocity() const
{
	assert(body);
	return Vec3ReactToRhy(body->getLinearVelocity());
}

void RigidBody::SetLinearVelocity(const RhyM::Vec3& linearVelocity)
{
	assert(body);
	body->setLinearVelocity(Vec3RhyToReact(linearVelocity));
}

RhyM::Vec3 RigidBody::GetAngularVelocity() const
{
	assert(body);
	return Vec3ReactToRhy(body->getAngularVelocity());
}

void RigidBody::SetAngularVelocity(const RhyM::Vec3& angularVelocity)
{
	assert(body);
	body->setAngularVelocity(Vec3RhyToReact(angularVelocity));
}

const RhyM::Vec3 RigidBody::GetLocalInertiaTensor() const
{
	assert(body);
	return Vec3ReactToRhy(body->getLocalInertiaTensor());
}

void RigidBody::SetLocalInertiaTensor(const RhyM::Vec3& inertiaTensorLocal)
{
	assert(body);
	body->setLocalInertiaTensor(Vec3RhyToReact(inertiaTensorLocal));
}

const RhyM::Vec3 RigidBody::GetLocalCenterOfMass() const
{
	assert(body);
	return Vec3ReactToRhy(body->getLocalCenterOfMass());
}

void RigidBody::SetLocalCenterOfMass(const RhyM::Vec3& centerOfMass)
{
	assert(body);
	body->setLocalCenterOfMass(Vec3RhyToReact(centerOfMass));
}

void RigidBody::UpdateLocalCenterOfMassFromColliders()
{
	assert(body);
	body->updateLocalCenterOfMassFromColliders();
}

void RigidBody::UpdateLocalInertiaTensorFromColliders()
{
	assert(body);
	body->updateLocalInertiaTensorFromColliders();
}

void RigidBody::UpdateMassFromColliders()
{
	assert(body);
	body->updateMassFromColliders();
}

void RigidBody::UpdateMassPropertiesFromColliders()
{
	assert(body);
	body->updateMassPropertiesFromColliders();
}

reactphysics3d::BodyType RigidBody::GetType() const
{
	assert(body);
	return body->getType();
}

void RigidBody::SetType(reactphysics3d::BodyType type)
{
	assert(body);
	body->setType(type);
}

bool RigidBody::IsGravityEnabled() const
{
	assert(body);
	return body->isGravityEnabled();
}

void RigidBody::EnableGravity(bool isEnabled)
{
	assert(body);
	body->enableGravity(isEnabled);
}

float RigidBody::GetLinearDamping() const
{
	assert(body);
	return body->getLinearDamping();
}

void RigidBody::SetLinearDamping(float linearDamping)
{
	assert(body);
	body->setLinearDamping(linearDamping);
}

float RigidBody::GetAngularDamping() const
{
	assert(body);
	return body->getAngularDamping();
}

void RigidBody::SetAngularDamping(float angularDamping)
{
	assert(body);
	body->setAngularDamping(angularDamping);
}

void RigidBody::ApplyForceToCenterOfMass(const RhyM::Vec3& force)
{
	assert(body);
	body->applyForceToCenterOfMass(Vec3RhyToReact(force));
}

void RigidBody::ApplyForceAtLocalPosition(const RhyM::Vec3& force, const RhyM::Vec3& point)
{
	assert(body);
	body->applyForceAtLocalPosition(Vec3RhyToReact(force), Vec3RhyToReact(point));
}

void RigidBody::ApplyForceAtWorldPosition(const RhyM::Vec3& force, const RhyM::Vec3& point)
{
	assert(body);
	body->applyForceAtWorldPosition(Vec3RhyToReact(force), Vec3RhyToReact(point));
}

void RigidBody::ApplyTorque(const RhyM::Vec3& torque)
{
	assert(body);
	body->applyTorque(Vec3RhyToReact(torque));
}

bool RigidBody::IsAllowedToSleep() const
{
	assert(body);
	return body->isAllowedToSleep();
}

void RigidBody::SetIsAllowedToSleep(bool isAllowedToSleep)
{
	assert(body);
	body->setIsAllowedToSleep(isAllowedToSleep);
}

bool RigidBody::IsSleeping() const
{
	assert(body);
	return body->isSleeping();
}

void RigidBody::OnEnabled()
{
	assert(body);
	body->setIsActive(true);
}

void RigidBody::OnDisabled()
{
	assert(body);
	body->setIsActive(false);
}

void RigidBody::UpdatePosition()
{
	const reactphysics3d::Transform& reactTransform = body->getTransform();
	transform->SetWorldPosition(Vec3ReactToRhy(reactTransform.getPosition()));
	transform->SetWorldRotation(QuatReactToRhy(reactTransform.getOrientation()));
}
