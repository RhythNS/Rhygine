#include <React/Collider/ConvexMeshCollider.h>
#include <React/ReactConverter.h>
#include <ECS/Components/Transform.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <reactphysics3d/collision/shapes/ConvexMeshShape.h>
#include <reactphysics3d/collision/PolyhedronMesh.h>

using namespace RhyReact;

RhyReact::ConvexMeshCollider::ConvexMeshCollider(reactphysics3d::PolyhedronMesh* polyhedronMesh, const RhyM::Vec3& scaling)
	: convexMeshShape(GetCommon()->createConvexMeshShape(polyhedronMesh, Vec3RhyToReact(scaling)))
{
}

const RhyM::Vec3 RhyReact::ConvexMeshCollider::GetScale()
{
	assert(convexMeshShape);
	return Vec3ReactToRhy(convexMeshShape->getScale());
}

void RhyReact::ConvexMeshCollider::SetScale(const RhyM::Vec3& scale)
{
	assert(convexMeshShape);
	convexMeshShape->setScale(Vec3RhyToReact(scale));
	GetTransform()->SetWorldScale(scale);
}

void RhyReact::ConvexMeshCollider::Release(reactphysics3d::PhysicsCommon* common)
{
	assert(convexMeshShape);
	common->destroyConvexMeshShape(convexMeshShape);
	convexMeshShape = nullptr;
}

reactphysics3d::CollisionShape* RhyReact::ConvexMeshCollider::GetCollisionShape()
{
	return convexMeshShape;
}

reactphysics3d::ConvexPolyhedronShape* RhyReact::ConvexMeshCollider::GetConvexPolyhedronShape()
{
	return convexMeshShape;
}
