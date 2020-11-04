#include "Time.h"

Time::Time() : 
	lastTime(std::chrono::steady_clock::now()), 
	programStartTime(lastTime),
	delta(0),
	physicsUpdate(0.0f),
	physicsUpdateRate(0.02f)
{
}

float Time::GetDelta()
{
	return delta;
}

float Time::GetTimeSinceStart()
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - programStartTime).count();
}

void Time::StartOfFrame()
{
	using namespace std::chrono;

	steady_clock::time_point nowTime = steady_clock::now();
	duration<float> deltaTime = duration<float>(nowTime - lastTime);
	physicsUpdate += deltaTime;

	delta = deltaTime.count();
	lastTime = nowTime;
}

bool Time::ShouldUpdatePhysics()
{
	if ((physicsUpdate - physicsUpdateRate).count() >= 0)
	{
		physicsUpdate -= physicsUpdateRate;
		return true;
	}
	return false;
}
