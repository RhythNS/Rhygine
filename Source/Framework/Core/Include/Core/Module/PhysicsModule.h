#pragma once
#include <Core/Module/Module.h>

#include <chrono>

class PhysicsModule : public Module
{
public:
	virtual void StartOfFramePreUpdate() override;

protected:
	bool ShouldUpdatePhysics();

	void SetUpdateRate(std::chrono::duration<float> updateRate);
	std::chrono::duration<float> GetUpdateRate();


	virtual void TickSimulation() = 0;
	virtual void UpdatePositions() = 0;
};
