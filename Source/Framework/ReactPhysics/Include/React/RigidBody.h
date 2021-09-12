#pragma once
#include <React/ReactBodyBase.h>
#include <RhyMath.h>
#include <React/Collider/ReactCollider.h>
#include <reactphysics3d/components/RigidBodyComponents.h>
#include <vector>

class Transform;

namespace reactphysics3d { class RigidBody; class Collider; class CollisionShape; }

namespace RhyReact
{
	class RigidBody : public ReactBodyBase
	{
		friend class ReactPhysicsModule;
	public:
        ~RigidBody();

        void Init() override;

		float GetMass();

		void SetMass(float mass);

        RhyM::Vec3 GetLinearVelocity() const;

        void SetLinearVelocity(const RhyM::Vec3& linearVelocity);

        RhyM::Vec3 GetAngularVelocity() const;

        void SetAngularVelocity(const RhyM::Vec3& angularVelocity);

        const RhyM::Vec3 GetLocalInertiaTensor() const;

        void SetLocalInertiaTensor(const RhyM::Vec3& inertiaTensorLocal);

        const RhyM::Vec3 GetLocalCenterOfMass() const;

        void SetLocalCenterOfMass(const RhyM::Vec3& centerOfMass);

        void UpdateLocalCenterOfMassFromColliders();

        void UpdateLocalInertiaTensorFromColliders();

        void UpdateMassFromColliders();

        void UpdateMassPropertiesFromColliders();

        reactphysics3d::BodyType GetType() const;

        void SetType(reactphysics3d::BodyType type);

        bool IsGravityEnabled() const;

        void EnableGravity(bool isEnabled);

        float GetLinearDamping() const;

        void SetLinearDamping(float linearDamping);

        float GetAngularDamping() const;

        void SetAngularDamping(float angularDamping);

        void ApplyForceToCenterOfMass(const RhyM::Vec3& force);

        void ApplyForceAtLocalPosition(const RhyM::Vec3& force, const RhyM::Vec3& point);

        void ApplyForceAtWorldPosition(const RhyM::Vec3& force, const RhyM::Vec3& point);

        void ApplyTorque(const RhyM::Vec3& torque);

        bool IsAllowedToSleep() const;

        void SetIsAllowedToSleep(bool isAllowedToSleep);

        bool IsSleeping() const;

    protected:
        virtual void OnEnabled() override;

        virtual void OnDisabled() override;

    private:
		void UpdatePosition();

		reactphysics3d::RigidBody* body = nullptr;

		RigidBody* next = nullptr;
		RigidBody* prev = nullptr;

		Transform* transform = nullptr;
		bool inSimulation = false;
	};
}
