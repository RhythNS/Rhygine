#include <Bullet/BulletPhysicsModule.h>
#include <Bullet/RigidBody.h>
#include <Bullet/BulletDebugDraw.h>
#include <Bullet/BulletConverter.h>
#include <Core/Window.h>
#include <Core/Gfx.h>
#include <Core/Scene.h>
#include <ECS/Stage.h>

using namespace RhyBullet;

BulletPhysicsModule::BulletPhysicsModule(float secondsPerTick) :
	secondsPerTick(secondsPerTick)
{
}

BulletPhysicsModule::~BulletPhysicsModule()
{
	instance = nullptr;
}

void BulletPhysicsModule::Setup()
{
	instance = this;

	world.setGravity(btVector3(0.0f, -9.81f, 0.0f));

	SetUpdateRate(secondsPerTick);
}

void BulletPhysicsModule::Register(RigidBody* body)
{
	assert(body);
	instance->world.addRigidBody(body->body.get());
	body->inSimulation = true;

	// Body is not the first to be added
	if (instance->first != nullptr)
	{
		instance->first->prev = body;
		body->next = instance->first;
	}
	instance->first = body;
}

void BulletPhysicsModule::DeRegister(RigidBody* body)
{
	assert(body);
	instance->world.removeRigidBody(body->body.get());
	body->inSimulation = false;

	// The body is the first body
	if (instance->first == body)
	{
		// There is a body after the body to delete
		if (body->next != nullptr)
		{
			instance->first = body->next;
			instance->first->prev = nullptr;
		}
		// There is no body after the body to delete
		else
		{
			instance->first = nullptr;
		}
		return;
	}
	// The body is not the first body

	// The body is the last in the list
	if (body->next == nullptr)
		body->prev->next = nullptr;
	// The body is not the last in the list
	else
	{
		body->prev->next = body->next;
		body->next->prev = body->prev;
	}
}

void BulletPhysicsModule::LateDraw()
{
	// If physics is enabled and the debug mode is enabled then draw the wireframes of
// the physics objects.
	if (IsDebugEnabled())
	{
		GetGfx()->ClearDepth();
		DebugDraw();
	}
}

void BulletPhysicsModule::TickSimulation()
{
	world.stepSimulation(secondsPerTick, 1, secondsPerTick);
}

void BulletPhysicsModule::UpdatePositions()
{
	// iterate over each body and call UpdatePosition.
	RigidBody* current = first;
	while (current != nullptr)
	{
		current->UpdatePosition();
		current = current->next;
	}
}

void BulletPhysicsModule::SetUpdateRate(float _secondsPerTick)
{
	PhysicsModule::SetUpdateRate(std::chrono::duration<float>(_secondsPerTick));
	secondsPerTick = _secondsPerTick;
}

void BulletPhysicsModule::SetGravity(RhyM::Vec3& gravity)
{
	world.setGravity(Vec3ToBullet(gravity));
}

RhyM::Vec3 BulletPhysicsModule::GetGravity()
{
	return Vec3ToRhy(world.getGravity());
}

void BulletPhysicsModule::OnSceneChange(Scene* scene)
{
	if (debugMode)
	{
		DisableDebug();
	}
	EnableDebug(GetStage(scene));
}

void BulletPhysicsModule::EnableDebug(Stage* stage)
{
	// Is the debug drawer already enabled?
	if (!instance->debugDraw)
	{
		instance->debugDraw = std::make_unique<BulletDebugDraw>();
		instance->debugDraw->Init(instance->GetCurrentStage());
	}
	instance->debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	instance->world.setDebugDrawer(instance->debugDraw.get());
	instance->debugMode = true;
}

void BulletPhysicsModule::DisableDebug()
{
	instance->world.setDebugDrawer(nullptr);
	instance->debugMode = false;
	// Is there a debug drawer currently active?
	if (instance->debugDraw)
	{
		instance->debugDraw.release();
	}
}

bool BulletPhysicsModule::IsDebugEnabled()
{
	return instance->debugMode;
}

btDiscreteDynamicsWorld* BulletPhysicsModule::GetWorld()
{
	return &world;
}

void BulletPhysicsModule::DebugDraw()
{
	world.debugDrawWorld();
}

BulletPhysicsModule* BulletPhysicsModule::instance;
