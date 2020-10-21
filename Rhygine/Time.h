#pragma once
#include <chrono>

#include "Tickable.h"

/// <summary>
/// Holds time related information about the simulation.
/// </summary>
class Time : public Tickable
{
	friend class Window;
public:
	Time();
	/// <summary>
	/// Gets the delta time since last frame in seconds.
	/// </summary>
	/// <returns></returns>
	float GetDelta();
	/// <summary>
	/// Gets the total amount of time since program start in seconds.
	/// </summary>
	/// <returns></returns>
	float GetTimeSinceStart();

private:
	/// <summary>
	/// Updates the delta time.
	/// </summary>
	void Tick();

	std::chrono::steady_clock::time_point programStartTime;
	std::chrono::steady_clock::time_point lastTime;

	float delta;
};
