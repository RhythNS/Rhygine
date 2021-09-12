#pragma once
#include <RhyMath.h>
#include <React/Ray.h>
#include <React/RaycastInfo.h>
#include <reactphysics3d/engine/Material.h>

namespace reactphysics3d { class PhysicsCommon; class CollisionShape; class Collider; }

class Transform;

namespace RhyReact
{
	class ReactBodyBase;

	class ReactCollider
	{
		friend class ReactBodyBase;

	public:
        void GetLocalToBodyPositionAndRotation(RhyM::Vec3& position, RhyM::Quat& quat);

        void SetLocalToBodyPositionAndRotation(RhyM::Vec3& position, RhyM::Quat& quat);

        void GetLocalToWorldPositionAndRotation(RhyM::Vec3& position, RhyM::Quat& quat);

        bool TestPointInside(const RhyM::Vec3& worldPoint);

        bool Raycast(const Ray& ray, RaycastInfo& raycastInfo);

        unsigned short GetCollideWithMaskBits() const;

        void SetCollideWithMaskBits(unsigned short collideWithMaskBits);

        unsigned short GetCollisionCategoryBits() const;

        void SetCollisionCategoryBits(unsigned short collisionCategoryBits);

		reactphysics3d::Material& GetMaterial();

        void SetMaterial(const reactphysics3d::Material& material);

        bool GetIsTrigger() const;

        void SetIsTrigger(bool isTrigger) const;

		ReactBodyBase* GetAttachedToBody();

		RhyM::Vec3 GetLocalInertiaTensor(float mass);

		float GetVolume();

		bool IsConvex();

		bool IsPolyhedron();

		void GetLocalBounds(RhyM::Vec3& min, RhyM::Vec3& max);

	protected:
		virtual void Release(reactphysics3d::PhysicsCommon* common) = 0;
		
		virtual reactphysics3d::CollisionShape* GetCollisionShape() = 0;

		reactphysics3d::PhysicsCommon* GetCommon();

		Transform* GetTransform();

		ReactBodyBase* attachedTo = nullptr;
		
	private:
		reactphysics3d::Collider* collider = nullptr;
		bool isShared = false;
	};
}
