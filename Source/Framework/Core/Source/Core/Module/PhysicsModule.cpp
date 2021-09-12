#include <Core/Module/PhysicsModule.h>
#include <Core/Time.h>
#include <Core/Window.h>

bool PhysicsModule::ShouldUpdatePhysics()
{
	return GetWindow()->time.ShouldUpdatePhysics();
}

void PhysicsModule::SetUpdateRate(std::chrono::duration<float> updateRate)
{
	GetWindow()->time.physicsUpdateRate = updateRate;
}

std::chrono::duration<float> PhysicsModule::GetUpdateRate()
{
	return GetWindow()->time.physicsUpdateRate;
}

void PhysicsModule::StartOfFramePreUpdate()
{
	bool moved = false;
	while (ShouldUpdatePhysics())
	{
		TickSimulation();
		moved = true;
	}
	if (moved)
		UpdatePositions();
}
