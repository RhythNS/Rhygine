#pragma once

#include <React/Collider/ReactCollider.h>

namespace reactphysics3d { class ConvexShape; }

namespace RhyReact
{
	class ConvexCollider : public ReactCollider
	{
	public:
		float GetMargin();

	protected:
		virtual void Release(reactphysics3d::PhysicsCommon* common) = 0;

		virtual reactphysics3d::CollisionShape* GetCollisionShape() = 0;

		virtual reactphysics3d::ConvexShape* GetConvexShape() = 0;
	};
}
