#pragma once

#include <React/Collider/ConcaveCollider.h>
#include <reactphysics3d/collision/shapes/HeightFieldShape.h>
#include <RhyMath.h>

namespace RhyReact
{
	class HeightFieldCollider : public ConcaveCollider
	{
	public:
		HeightFieldCollider(int numGridColumns, int numGridRows, float minHeight, float maxHeight,
			const void* heightFieldData, reactphysics3d::HeightFieldShape::HeightDataType dataType,
			int upAxis = 1, float integerHeightScale = 1.0f,
			const reactphysics3d::Vector3& scaling = reactphysics3d::Vector3(1, 1, 1));

		int GetNumRows() const;

		int GetNumColumns() const;

		reactphysics3d::Vector3 GetVertexAt(int x, int y) const;

		float GetHeightAt(int x, int y) const;

		reactphysics3d::HeightFieldShape::HeightDataType GetHeightDataType() const;

	protected:
		virtual void Release(reactphysics3d::PhysicsCommon* common) override;

		virtual reactphysics3d::CollisionShape* GetCollisionShape();

		virtual reactphysics3d::ConcaveShape* GetConcaveShape();

		reactphysics3d::HeightFieldShape* heightFieldShape = nullptr;
	};
}
