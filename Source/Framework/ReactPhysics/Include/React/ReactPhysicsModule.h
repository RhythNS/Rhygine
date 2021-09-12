#pragma once
#include <Core/Module/PhysicsModule.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <React/ReactLogger.h>

namespace RhyReact
{
	class RigidBody;
	class ReactCollider;
	class ReactBodyBase;
	class ReactDebugRenderer;

	class ReactPhysicsModule : public PhysicsModule
	{
		friend class ReactBodyBase;
		friend class ReactCollider;

	public:
		ReactPhysicsModule();
		ReactPhysicsModule(float updateRate = 0.02f, reactphysics3d::PhysicsWorld::WorldSettings settings = reactphysics3d::PhysicsWorld::WorldSettings());
		~ReactPhysicsModule();

		virtual void Setup() override;

		static void Register(RigidBody* body);

		static void DeRegister(RigidBody* body);

		virtual void LateDraw() override;

		virtual void OnSceneChange(Scene* scene) override;

		void EnableDebug(Scene* scene);

		void DisableDebug();

		bool IsDebugEnabled();

	protected:
		virtual void TickSimulation() override;
		virtual void UpdatePositions() override;

	private:
		float updateRate = 0.02f;
		RigidBody* front = nullptr;

		ReactLogger logger{};
		reactphysics3d::PhysicsCommon common{};
		reactphysics3d::PhysicsWorld* world = nullptr;
		static ReactPhysicsModule* instance;
		ReactDebugRenderer* debugRenderer = nullptr;
	};
}
