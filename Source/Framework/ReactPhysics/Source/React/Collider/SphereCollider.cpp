#include <React/Collider/SphereCollider.h>
#include <reactphysics3d/collision/shapes/SphereShape.h>
#include <reactphysics3d/engine/PhysicsCommon.h>

using namespace RhyReact;

RhyReact::SphereCollider::SphereCollider(const float radius)
	: sphereShape(GetCommon()->createSphereShape(radius))
{
}

float RhyReact::SphereCollider::GetRadius()
{
	assert(sphereShape);
	return sphereShape->getRadius();
}

void RhyReact::SphereCollider::SetRadius(float radius)
{
	assert(sphereShape);
	sphereShape->setRadius(radius);
}

void RhyReact::SphereCollider::Release(reactphysics3d::PhysicsCommon* common)
{
	assert(sphereShape);
	common->destroySphereShape(sphereShape);
	sphereShape = nullptr;
}

reactphysics3d::CollisionShape* RhyReact::SphereCollider::GetCollisionShape()
{
	return sphereShape;
}

reactphysics3d::ConvexShape* RhyReact::SphereCollider::GetConvexShape()
{
	return sphereShape;
}
