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
	/// Should be called at the start of the frame.
	/// </summary>
	void StartOfFrame();

	/// <summary>
	/// Should be called at the end of the frame.
	/// </summary>
	void EndOfFrame();

	/// <summary>
	/// Gets the time in seconds the program should wait before the next frame should start
	/// to be proccessed.
	/// </summary>
	std::chrono::duration<float> GetSleepTime();

	/// <summary>
	/// Returns true when the physics should do a simulation step.
	/// </summary>
	bool ShouldUpdatePhysics();

	std::chrono::steady_clock::time_point programStartTime;
	std::chrono::steady_clock::time_point frameStartTime;

	std::chrono::duration<float> frameDuration = std::chrono::microseconds(0);
	std::chrono::duration<float> wantedFrameTime = std::chrono::microseconds(0);

	std::chrono::duration<float> physicsUpdate = std::chrono::microseconds(0);
	std::chrono::duration<float> physicsUpdateRate;

	float delta;
};
