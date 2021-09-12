#pragma once
#include <ECS/Component.h>
#include <RhyMath.h>
#include <reactphysics3d/components/CollisionBodyComponents.h>
#include <React/Collider/ReactCollider.h>
#include <React/Ray.h>
#include <React/RaycastInfo.h>
#include <vector>

class Transform;

namespace reactphysics3d { class PhysicsWorld; class PhysicsCommon; }

namespace RhyReact
{
	class ReactBodyBase : public Component
	{
	public:
		virtual ~ReactBodyBase();

		virtual void Init() override = 0;

		void Move(RhyM::Vec3 position, RhyM::Quat quat);

		void GetPositionAndRotation(RhyM::Vec3& position, RhyM::Quat& quat) const;

		void SetPositionAndRotation(const RhyM::Vec3& position, const RhyM::Quat& quat);

		bool TestPointInside(const RhyM::Vec3& worldPoint) const;

		bool Raycast(const Ray& ray, RaycastInfo& raycastInfo);

		RhyM::Vec3 GetWorldPoint(const RhyM::Vec3& localPoint) const;

		RhyM::Vec3 GetWorldVector(const RhyM::Vec3& localVector) const;

		RhyM::Vec3 GetLocalPoint(const RhyM::Vec3& worldPoint) const;

		RhyM::Vec3 GetLocalVector(const RhyM::Vec3& worldVector) const;

		bool AddCollider(ReactCollider* collider, const RhyM::Vec3& position, const RhyM::Quat& quat, const bool transferOwnerShip = true);

		void RemoveCollider(ReactCollider* collider);

		int GetColliderNum();

		ReactCollider* GetColliderAt(int index);

		Transform* GetTransform();

		template <class T>
		T* GetCollider()
		{
			T* t;
			for (auto* collider : colliders)
			{
				if (t = dynamic_cast<T*>(collider))
					return t;
			}
			return nullptr;
		}

	protected:
		virtual void OnEnabled() override = 0;
		virtual void OnDisabled() override = 0;

		reactphysics3d::PhysicsWorld* GetWorld();
		reactphysics3d::PhysicsCommon* GetCommon();

		reactphysics3d::CollisionBody* collisionBody = nullptr;

		Transform* transform = nullptr;

	private:

		struct ColliderOwned
		{
		public:
			ReactCollider* collider = nullptr;
			bool isOwned = false;

			ColliderOwned(ReactCollider* collider, bool isOwned)
				: collider(collider), isOwned(isOwned)
			{
			}
		};

		reactphysics3d::Collider* AddCollider(reactphysics3d::CollisionShape* collisionShape, const RhyM::Vec3& position, const RhyM::Quat& quat);

		void InnerRemoveCollider(ReactCollider* toRemove);

		std::vector<ColliderOwned> colliders;
	};
}
