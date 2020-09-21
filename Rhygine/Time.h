#pragma once
#include <chrono>

#include "Tickable.h"

class Time : public Tickable
{
	friend class Window;
public:
	Time();
	 float GetDelta();
	 float GetTimeSinceStart();

private:
	void Tick();

	std::chrono::steady_clock::time_point programStartTime;
	std::chrono::steady_clock::time_point lastTime;

	float delta;
};

