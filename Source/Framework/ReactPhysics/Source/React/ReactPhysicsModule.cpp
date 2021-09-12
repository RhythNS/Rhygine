#include <React/ReactPhysicsModule.h>
#include <React/RigidBody.h>
#include <React/ReactConverter.h>
#include <React/ReactDebugRenderer.h>

#include <Core/Gfx.h>
#include <ECS/Components/Transform.h>
#include <chrono>

using namespace RhyReact;

ReactPhysicsModule::ReactPhysicsModule() : common(), world(common.createPhysicsWorld())
{
	assert(instance == nullptr);
	instance = this;
}

ReactPhysicsModule::ReactPhysicsModule(float updateRate, reactphysics3d::PhysicsWorld::WorldSettings settings)
	: common(), world(common.createPhysicsWorld(settings)), updateRate(updateRate)
{
	assert(instance == nullptr);
	instance = this;
}

ReactPhysicsModule::~ReactPhysicsModule()
{
	instance = nullptr;
	if (debugRenderer)
		delete debugRenderer;
}

void ReactPhysicsModule::Setup()
{
	SetUpdateRate(std::chrono::duration<float>(updateRate));
	common.setLogger(&logger);
}

void ReactPhysicsModule::Register(RigidBody* body)
{
	assert(body);
	body->inSimulation = true;

	// Body is not the first to be added
	if (instance->front != nullptr)
	{
		instance->front->prev = body;
		body->next = instance->front;
	}
	instance->front = body;
}

void ReactPhysicsModule::DeRegister(RigidBody* body)
{
	assert(body);
	body->inSimulation = false;

	// The body is the first body
	if (instance->front == body)
	{
		// There is a body after the body to delete
		if (body->next != nullptr)
		{
			instance->front = body->next;
			instance->front->prev = nullptr;
		}
		// There is no body after the body to delete
		else
		{
			instance->front = nullptr;
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

void ReactPhysicsModule::LateDraw()
{
	if (IsDebugEnabled() == false)
		return;

	GetGfx()->ClearDepth();
	debugRenderer->Draw(world->getDebugRenderer());
}

void ReactPhysicsModule::OnSceneChange(Scene* scene)
{
	if (IsDebugEnabled() == false)
		return;

	delete debugRenderer;
	debugRenderer = new	ReactDebugRenderer();
	debugRenderer->Init(GetStage(scene));
}

void ReactPhysicsModule::EnableDebug(Scene* scene)
{
	if (IsDebugEnabled())
		return;

	world->setIsDebugRenderingEnabled(true);
	debugRenderer = new ReactDebugRenderer();
	debugRenderer->Init(GetStage(scene));
	
	reactphysics3d::DebugRenderer& debug = world->getDebugRenderer();
	debug.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
	debug.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	debug.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
}

void ReactPhysicsModule::DisableDebug()
{
	if (IsDebugEnabled() == false)
		return;

	world->setIsDebugRenderingEnabled(false);
	delete debugRenderer;
	debugRenderer = nullptr;
}

bool ReactPhysicsModule::IsDebugEnabled()
{
	return debugRenderer != nullptr;
}

void ReactPhysicsModule::TickSimulation()
{
	world->update(updateRate);
}

void ReactPhysicsModule::UpdatePositions()
{
	RigidBody* current = front;
	while (current != nullptr)
	{
		if (current->inSimulation)
			current->UpdatePosition();
		current = current->next;
	}
}

ReactPhysicsModule* ReactPhysicsModule::instance = nullptr;
