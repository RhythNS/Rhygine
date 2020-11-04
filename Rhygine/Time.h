#pragma once
#include <chrono>

#include "Tickable.h"

/// <summary>
/// Holds time related information about the simulation.
/// </summary>
class Time
{
	friend class Window;
	friend class Physics;
public:
	Time();
	/// <summary>
	/// Gets the delta time since last frame in seconds.
	/// </summary>
	float GetDelta();
	/// <summary>
	/// Gets the total amount of time since program start in seconds.
	/// </summary>
	float GetTimeSinceStart();

private:
	/// <summary>
	/// Updates the delta time.
	/// </summary>
	void StartOfFrame();

	/// <summary>
	/// Returns true when the physics should do a simulation step.
	/// </summary>
	bool ShouldUpdatePhysics();

	std::chrono::steady_clock::time_point programStartTime;
	std::chrono::steady_clock::time_point lastTime;
	std::chrono::duration<float> physicsUpdate;
	std::chrono::duration<float> physicsUpdateRate;

	float delta;
};
