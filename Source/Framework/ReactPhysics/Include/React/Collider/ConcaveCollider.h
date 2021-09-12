#pragma once

#include <React/Collider/ReactCollider.h>
#include <reactphysics3d/collision/shapes/ConcaveShape.h>

namespace RhyReact
{
	class ConcaveCollider : public ReactCollider
	{
	public:
		reactphysics3d::TriangleRaycastSide GetRaycastTestType();

		void SetRaycastTestType(reactphysics3d::TriangleRaycastSide testType);

		const RhyM::Vec3 GetScale();

		void SetScale(const RhyM::Vec3& scale);

	protected:
		virtual void Release(reactphysics3d::PhysicsCommon* common) override = 0;
		
		virtual reactphysics3d::CollisionShape* GetCollisionShape() = 0;
		
		virtual reactphysics3d::ConcaveShape* GetConcaveShape() = 0;
	};
}
