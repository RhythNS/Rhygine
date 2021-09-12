#pragma once

#include <React/Collider/ConvexPolyhedronCollider.h>

namespace reactphysics3d { class ConvexMeshShape; class PolyhedronMesh; }

namespace RhyReact
{
	class ConvexMeshCollider : public ConvexPolyhedronCollider
	{
	public:
		ConvexMeshCollider(reactphysics3d::PolyhedronMesh* polyhedronMesh, const RhyM::Vec3& scaling = RhyM::Vec3(1, 1, 1));

		const RhyM::Vec3 GetScale();

		void SetScale(const RhyM::Vec3& scale);

	protected:
		virtual void Release(reactphysics3d::PhysicsCommon* common);

		virtual reactphysics3d::CollisionShape* GetCollisionShape();

		virtual reactphysics3d::ConvexPolyhedronShape* GetConvexPolyhedronShape();

		reactphysics3d::ConvexMeshShape* convexMeshShape = nullptr;
	};
}
