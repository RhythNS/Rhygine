#pragma once
#include "bullet\btBulletDynamicsCommon.h"
#include "RhyMath.h"

#include <memory>

class RigidBody;
class BulletDebugDraw;
class Stage;
class Physics
{
	friend class Window;
public:
	Physics(float secondsPerTick);

	static void Register(RigidBody* body);
	static void DeRegister(RigidBody* body);

	void Tick();
	void UpdatePositions();

	void SetUpdateRate(float secondsPerTick);
	void SetGravity(RhyM::Vec3& gravity);
	RhyM::Vec3 GetGravity();

	static void EnableDebug(Stage* stage);
	static void DisableDebug();
	static bool IsDebugEnabled();
private:
	void DebugDraw();

	static Physics* instance;
	btDbvtBroadphase broadphase;
	btDefaultCollisionConfiguration collisionConfiguration;
	btCollisionDispatcher dispatcher{ &collisionConfiguration };
	btSequentialImpulseConstraintSolver constraintSolver;
	btDiscreteDynamicsWorld world{ &dispatcher, &broadphase, &constraintSolver, &collisionConfiguration };
	RigidBody* first = nullptr;
	float secondsPerTick = 0.02f;

	bool debugMode = false;
	std::unique_ptr<BulletDebugDraw> debugDraw = nullptr;
};
