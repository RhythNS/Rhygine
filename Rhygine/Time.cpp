#include "Time.h"

Time::Time() : 
	frameStartTime(std::chrono::steady_clock::now()), 
	programStartTime(std::chrono::steady_clock::now()),
	delta(0),
	physicsUpdateRate(0.02f)
{
}

float Time::GetDelta()
{
	return delta;
}

float Time::GetTimeSinceStart()
{
	using namespace std::chrono;

	return duration<float>(steady_clock::now() - programStartTime).count();
}

void Time::StartOfFrame()
{
	using namespace std::chrono;

	steady_clock::time_point nowTime = steady_clock::now();
	duration<float> deltaTime = duration<float>(nowTime - frameStartTime);
	physicsUpdate += deltaTime;

	delta = deltaTime.count();
	frameStartTime = nowTime;
}

void Time::EndOfFrame()
{
	using namespace std::chrono;

	frameDuration = steady_clock::now() - frameStartTime;
}

std::chrono::duration<float> Time::GetSleepTime()
{
	return wantedFrameTime - frameDuration;
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
