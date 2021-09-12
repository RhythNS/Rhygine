#include <React/Collider/ReactCollider.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <React/ReactPhysicsModule.h>
#include <React/ReactConverter.h>
#include <reactphysics3d/mathematics/Ray.h>
#include <reactphysics3d/collision/RaycastInfo.h>
#include <ECS/Components/Transform.h>

using namespace RhyReact;

void ReactCollider::SetLocalToBodyPositionAndRotation(RhyM::Vec3& position, RhyM::Quat& quat)
{
	assert(collider);
	collider->setLocalToBodyTransform(TransformRhyToReact(position, quat));
}

void ReactCollider::GetLocalToWorldPositionAndRotation(RhyM::Vec3& position, RhyM::Quat& quat)
{
	assert(collider);
	TransformReactToRhy(collider->getLocalToWorldTransform(), position, quat);
}

bool ReactCollider::TestPointInside(const RhyM::Vec3& worldPoint)
{
	assert(collider);
	return collider->testPointInside(Vec3RhyToReact(worldPoint));
}

bool ReactCollider::Raycast(const Ray& ray, RaycastInfo& raycastInfo)
{
	assert(collider);
	reactphysics3d::Ray reactRay = RayRhyToReact(ray);
	reactphysics3d::RaycastInfo info;
	if (collider->raycast(reactRay, info))
	{
		raycastInfo = RaycastInfoReactToRhy(info);
		return true;
	}
	return false;
}

unsigned short ReactCollider::GetCollideWithMaskBits() const
{
	assert(collider);
	return collider->getCollideWithMaskBits();
}

void ReactCollider::SetCollideWithMaskBits(unsigned short collideWithMaskBits)
{
	assert(collider);
	collider->setCollideWithMaskBits(collideWithMaskBits);
}

unsigned short ReactCollider::GetCollisionCategoryBits() const
{
	assert(collider);
	return collider->getCollisionCategoryBits();
}

void ReactCollider::SetCollisionCategoryBits(unsigned short collisionCategoryBits)
{
	assert(collider);
	collider->setCollisionCategoryBits(collisionCategoryBits);
}

reactphysics3d::Material& ReactCollider::GetMaterial()
{
	assert(collider);
	return collider->getMaterial();
}

void ReactCollider::SetMaterial(const reactphysics3d::Material& material)
{
	assert(collider);
	collider->setMaterial(material);
}

bool ReactCollider::GetIsTrigger() const
{
	assert(collider);
	return collider->getIsTrigger();
}

void ReactCollider::SetIsTrigger(bool isTrigger) const
{
	assert(collider);
	collider->setIsTrigger(isTrigger);
}

ReactBodyBase* ReactCollider::GetAttachedToBody()
{
	return attachedTo;
}

RhyM::Vec3 ReactCollider::GetLocalInertiaTensor(float mass)
{
	return Vec3ReactToRhy(GetCollisionShape()->getLocalInertiaTensor(mass));
}

float ReactCollider::GetVolume()
{
	return GetCollisionShape()->getVolume();
}

bool ReactCollider::IsConvex()
{
	return GetCollisionShape()->isConvex();
}

bool ReactCollider::IsPolyhedron()
{
	return GetCollisionShape()->isPolyhedron();
}

void ReactCollider::GetLocalBounds(RhyM::Vec3& min, RhyM::Vec3& max)
{
	reactphysics3d::Vector3 reactMin, reactMax;
	GetCollisionShape()->getLocalBounds(reactMin, reactMax);
	min = Vec3ReactToRhy(reactMin);
	max = Vec3ReactToRhy(reactMax);
}

reactphysics3d::PhysicsCommon* ReactCollider::GetCommon()
{
	return &ReactPhysicsModule::instance->common;
}

Transform* ReactCollider::GetTransform()
{
	return attachedTo->GetTransform();
}
