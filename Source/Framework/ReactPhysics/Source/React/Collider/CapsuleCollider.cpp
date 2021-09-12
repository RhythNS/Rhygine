#include <React/Collider/CapsuleCollider.h>
#include <reactphysics3d/engine/PhysicsCommon.h>

using namespace RhyReact;

CapsuleCollider::CapsuleCollider(float radius, float height)
	: capsuleShape(GetCommon()->createCapsuleShape(radius, height))
{
}

float CapsuleCollider::GetRadius()
{
	assert(capsuleShape);
	return capsuleShape->getRadius();
}

void CapsuleCollider::SetRadius(float radius)
{
	assert(capsuleShape);
	capsuleShape->setRadius(radius);
}

float CapsuleCollider::GetHeight()
{
	assert(capsuleShape);
	return capsuleShape->getHeight();
}

void CapsuleCollider::SetHeight(float height)
{
	assert(capsuleShape);
	capsuleShape->setHeight(height);
}

void CapsuleCollider::Release(reactphysics3d::PhysicsCommon* common)
{
	assert(capsuleShape);
	common->destroyCapsuleShape(capsuleShape);
	capsuleShape = nullptr;
}

reactphysics3d::CollisionShape* RhyReact::CapsuleCollider::GetCollisionShape()
{
	return capsuleShape;
}

reactphysics3d::ConvexShape* RhyReact::CapsuleCollider::GetConvexShape()
{
	return capsuleShape;
}
