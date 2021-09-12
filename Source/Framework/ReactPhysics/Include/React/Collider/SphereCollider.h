#pragma once

#include <React/Collider/ConvexCollider.h>

namespace reactphysics3d { class SphereShape; }

namespace RhyReact
{
	class SphereCollider : public ConvexCollider
	{
	public:
		SphereCollider(const float radius);

		float GetRadius();

		void SetRadius(float radius);

	protected:
		virtual void Release(reactphysics3d::PhysicsCommon* common);

		virtual reactphysics3d::CollisionShape* GetCollisionShape();

		virtual reactphysics3d::ConvexShape* GetConvexShape();
	
		reactphysics3d::SphereShape* sphereShape = nullptr;
	};
}
