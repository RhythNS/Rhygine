#pragma once
#include "RhyBullet.h"
#include "RhyMath.h"
#include "BulletDebugDraw.h"

#include <memory>

class RigidBody;
class Stage;

/// <summary>
/// Wrapper class around the Bullet physics engine.
/// </summary>
class Physics
{
	friend class Window;
public:
	/// <summary>
	/// Standard constructor.
	/// </summary>
	/// <param name="secondsPerTick">The simulation step time in seconds.</param>
	Physics(float secondsPerTick);

	virtual ~Physics() { }

	/// <summary>
	/// Registers a Rigidbody to the physics system.
	/// </summary>
	/// <param name="body">The Rigidbody to be registered.</param>
	static void Register(RigidBody* body);
	/// <summary>
	/// Deregisters a Rigidbody from the physics system.
	/// </summary>
	/// <param name="body">The Rigidbody to be deregistered.</param>
	static void DeRegister(RigidBody* body);

	/// <summary>
	/// Steps the simulation forward by the specified seconds per tick.
	/// </summary>
	void Tick();
	/// <summary>
	/// Steps the simulation forward by specified delta time.
	/// </summary>
	void Tick(float delta);
	/// <summary>
	/// Converts the rigidbodies position from bullet to rhyghine.
	/// </summary>
	void UpdatePositions();

	/// <summary>
	/// Updates how many seconds a simulation step is.
	/// </summary>
	/// <param name="secondsPerTick">The simulation step time in seconds.</param>
	void SetUpdateRate(float secondsPerTick);
	/// <summary>
	/// Sets a new gravity for the physics simulation.
	/// </summary>
	void SetGravity(RhyM::Vec3& gravity);
	/// <summary>
	/// Gets the current gravity for the physics simulation.
	/// </summary>
	RhyM::Vec3 GetGravity();

	/// <summary>
	/// Enables the debug viewer.
	/// </summary>
	/// <param name="stage">The current stage of the program.</param>
	static void EnableDebug(Stage* stage);
	/// <summary>
	/// Disables the debug viewer.
	/// </summary>
	static void DisableDebug();
	/// <summary>
	/// Checks if the debug viewer is enabled or disabled.
	/// </summary>
	static bool IsDebugEnabled();

	/// <summary>
	/// Gets bullet world.
	/// </summary>
	btDiscreteDynamicsWorld* GetWorld();
private:
	/// <summary>
	/// Draws the debug viewer.
	/// </summary>
	void DebugDraw();

	static Physics* instance;
	btDbvtBroadphase broadphase;
	btDefaultCollisionConfiguration collisionConfiguration;
	btCollisionDispatcher dispatcher{ &collisionConfiguration };
	btSequentialImpulseConstraintSolver constraintSolver;
	btDiscreteDynamicsWorld world{ &dispatcher, &broadphase, &constraintSolver, &collisionConfiguration };
	RigidBody* first = nullptr;
	/// <summary>
	/// How many seconds a tick in bullet is.
	/// </summary>
	float secondsPerTick = 0.02f;

	bool debugMode = false;
	std::unique_ptr<BulletDebugDraw> debugDraw = nullptr;
};
