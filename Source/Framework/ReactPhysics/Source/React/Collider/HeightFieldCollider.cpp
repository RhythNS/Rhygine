#include <React/Collider/HeightFieldCollider.h>
#include <reactphysics3d/engine/PhysicsCommon.h>

using namespace RhyReact;

HeightFieldCollider::HeightFieldCollider(int numGridColumns, int numGridRows, float minHeight, float maxHeight,
	const void* heightFieldData, reactphysics3d::HeightFieldShape::HeightDataType dataType, int upAxis,
	float integerHeightScale, const reactphysics3d::Vector3& scaling)
	: heightFieldShape(GetCommon()->createHeightFieldShape(numGridColumns, numGridRows, minHeight, maxHeight, heightFieldData,
		dataType, upAxis, integerHeightScale, scaling))
{
}

int HeightFieldCollider::GetNumRows() const
{
	assert(heightFieldShape);
	return heightFieldShape->getNbRows();
}

int HeightFieldCollider::GetNumColumns() const
{
	assert(heightFieldShape);
	return heightFieldShape->getNbColumns();
}

reactphysics3d::Vector3 HeightFieldCollider::GetVertexAt(int x, int y) const
{
	assert(heightFieldShape);
	return heightFieldShape->getVertexAt(x, y);
}

float HeightFieldCollider::GetHeightAt(int x, int y) const
{
	assert(heightFieldShape);
	return heightFieldShape->getHeightAt(x, y);
}

reactphysics3d::HeightFieldShape::HeightDataType HeightFieldCollider::GetHeightDataType() const
{
	assert(heightFieldShape);
	return heightFieldShape->getHeightDataType();
}

void HeightFieldCollider::Release(reactphysics3d::PhysicsCommon* common)
{
	assert(heightFieldShape);
	common->destroyHeightFieldShape(heightFieldShape);
	heightFieldShape = nullptr;
}

reactphysics3d::CollisionShape* HeightFieldCollider::GetCollisionShape()
{
	return heightFieldShape;
}

reactphysics3d::ConcaveShape* HeightFieldCollider::GetConcaveShape()
{
	return heightFieldShape;
}
