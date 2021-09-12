#include <React/Collider/BoxCollider.h>
#include <reactphysics3d/collision/shapes/BoxShape.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <React/ReactConverter.h>

using namespace RhyReact;

RhyReact::BoxCollider::BoxCollider(const RhyM::Vec3& halfExtents)
	: boxShape(GetCommon()->createBoxShape(Vec3RhyToReact(halfExtents)))
{
}

RhyM::Vec3 RhyReact::BoxCollider::GetHalfExtents()
{
	assert(boxShape);
	return Vec3ReactToRhy(boxShape->getHalfExtents());
}

void RhyReact::BoxCollider::SetHalfExtents(const RhyM::Vec3& halfExtents)
{
	assert(boxShape);
	boxShape->setHalfExtents(Vec3RhyToReact(halfExtents));
}

void RhyReact::BoxCollider::Release(reactphysics3d::PhysicsCommon* common)
{
	assert(boxShape);
	common->destroyBoxShape(boxShape);
}

reactphysics3d::CollisionShape* RhyReact::BoxCollider::GetCollisionShape()
{
	return boxShape;
}

reactphysics3d::ConvexPolyhedronShape* RhyReact::BoxCollider::GetConvexPolyhedronShape()
{
	return boxShape;
}
