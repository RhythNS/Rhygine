#pragma once

#include <React/Collider/ConcaveCollider.h>
#include <RhyMath.h>

namespace reactphysics3d { class ConcaveMeshShape; class TriangleMesh; }

namespace RhyReact
{
	class ConcaveMeshCollider : public ConcaveCollider
	{
	public:
		ConcaveMeshCollider(reactphysics3d::TriangleMesh* triangleMesh, const RhyM::Vec3& scaling = RhyM::Vec3(1, 1, 1));

		unsigned int GetNumSubparts() const;

		unsigned int GetNumTriangles(unsigned int subPart) const;

		void GetTriangleVerticesIndices(unsigned int subPart, unsigned int triangleIndex, unsigned int* outVerticesIndices) const;

	protected:
		virtual void Release(reactphysics3d::PhysicsCommon* common) override;
		
		virtual reactphysics3d::CollisionShape* GetCollisionShape();

		virtual reactphysics3d::ConcaveShape* GetConcaveShape();

		reactphysics3d::ConcaveMeshShape* meshShape = nullptr;
	};
}
