#include <React/Collider/ConcaveMeshCollider.h>
#include <React/ReactConverter.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <reactphysics3d/collision/shapes/ConcaveMeshShape.h>
#include <reactphysics3d/collision/shapes/TriangleShape.h>

using namespace RhyReact;

ConcaveMeshCollider::ConcaveMeshCollider(reactphysics3d::TriangleMesh* triangleMesh, const RhyM::Vec3& scaling)
	: meshShape(GetCommon()->createConcaveMeshShape(triangleMesh, Vec3RhyToReact(scaling)))
{
}

unsigned int ConcaveMeshCollider::GetNumSubparts() const
{
	assert(meshShape);
	return meshShape->getNbSubparts();
}

unsigned int ConcaveMeshCollider::GetNumTriangles(unsigned int subPart) const
{
	assert(meshShape);
	return meshShape->getNbTriangles(subPart);
}

void ConcaveMeshCollider::GetTriangleVerticesIndices(unsigned int subPart, unsigned int triangleIndex, unsigned int* outVerticesIndices) const
{
	assert(meshShape);
	meshShape->getTriangleVerticesIndices(subPart, triangleIndex, outVerticesIndices);
}

void ConcaveMeshCollider::Release(reactphysics3d::PhysicsCommon* common)
{
	assert(meshShape);
	common->destroyConcaveMeshShape(meshShape);
	meshShape = nullptr;
}

reactphysics3d::CollisionShape* ConcaveMeshCollider::GetCollisionShape()
{
	return meshShape;
}

reactphysics3d::ConcaveShape* ConcaveMeshCollider::GetConcaveShape()
{
	return meshShape;
}
