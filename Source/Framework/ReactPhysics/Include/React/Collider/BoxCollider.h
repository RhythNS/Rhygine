#pragma once

#include <React/Collider/ConvexPolyhedronCollider.h>

namespace reactphysics3d { class BoxShape; }

namespace RhyReact
{
	class BoxCollider : public ConvexPolyhedronCollider
	{
	public:
		BoxCollider(const RhyM::Vec3& halfExtents);

		RhyM::Vec3 GetHalfExtents();

		void SetHalfExtents(const RhyM::Vec3& halfExtents);

	protected:
		virtual void Release(reactphysics3d::PhysicsCommon* common);

		virtual reactphysics3d::CollisionShape* GetCollisionShape();

		virtual reactphysics3d::ConvexPolyhedronShape* GetConvexPolyhedronShape();

		reactphysics3d::BoxShape* boxShape = nullptr;
	};
}
