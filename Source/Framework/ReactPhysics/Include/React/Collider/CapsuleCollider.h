#pragma once

#include <React/Collider/ConvexCollider.h>

namespace reactphysics3d { class CapsuleShape; }

namespace RhyReact
{
	class CapsuleCollider : public ConvexCollider
	{
	public:
		CapsuleCollider(float radius, float height);

		float GetRadius();

		void SetRadius(float radius);

		float GetHeight();

		void SetHeight(float height);

	protected:
		virtual void Release(reactphysics3d::PhysicsCommon* common);

		virtual reactphysics3d::CollisionShape* GetCollisionShape();

		virtual reactphysics3d::ConvexShape* GetConvexShape();

		reactphysics3d::CapsuleShape* capsuleShape = nullptr;
	};
}
