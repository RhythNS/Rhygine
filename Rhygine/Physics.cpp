#include "Physics.h"
#include "RigidBody.h"
#include "Window.h"
#include "BulletDebugDraw.h"

Physics::Physics(float secondsPerTick) :
	secondsPerTick(secondsPerTick)
{
	instance = this;

	world.setGravity(RhyM::Vec3(0.0f, -9.81f, 0.0f));

	Window::GetInstance()->time.physicsUpdateRate = std::chrono::duration<float>(secondsPerTick);
}

Physics::~Physics()
{
	instance = nullptr;
}

void Physics::Register(RigidBody* body)
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

void Physics::DeRegister(RigidBody* body)
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

void Physics::Tick()
{
	world.stepSimulation(secondsPerTick, 1, secondsPerTick);
}

void Physics::Tick(float delta)
{
	world.stepSimulation(delta, 1, delta);
}

void Physics::UpdatePositions()
{
	// iterate over each body and call UpdatePosition.
	RigidBody* current = first;
	while (current != nullptr)
	{
		current->UpdatePosition();
		current = current->next;
	}
}

void Physics::SetUpdateRate(float _secondsPerTick)
{
	Window::GetInstance()->time.physicsUpdateRate = std::chrono::duration<float>(_secondsPerTick);
	secondsPerTick = _secondsPerTick;
}

void Physics::SetGravity(RhyM::Vec3& gravity)
{
	world.setGravity(gravity);
}

RhyM::Vec3 Physics::GetGravity()
{
	return world.getGravity();
}

void Physics::EnableDebug(Stage* stage)
{
	// Is the debug drawer already enabled?
	if (!instance->debugDraw)
	{
		instance->debugDraw = std::make_unique<BulletDebugDraw>();
		instance->debugDraw->Init(stage);
	}
	instance->debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	instance->world.setDebugDrawer(instance->debugDraw.get());
	instance->debugMode = true;
}

void Physics::DisableDebug()
{
	instance->world.setDebugDrawer(nullptr);
	instance->debugMode = false;
	// Is there a debug drawer currently active?
	if (instance->debugDraw)
	{
		instance->debugDraw.release();
	}
}

bool Physics::IsDebugEnabled()
{
	return instance->debugMode;
}

btDiscreteDynamicsWorld* Physics::GetWorld()
{
	return &world;
}

void Physics::DebugDraw()
{
	world.debugDrawWorld();
}

Physics* Physics::instance;
