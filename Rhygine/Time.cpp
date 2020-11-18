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

float Time::LastFrameDuration()
{
	return frameDuration.count();
}

void Time::StartOfFrame()
{
	using namespace std::chrono;

	// The current time
	steady_clock::time_point nowTime = steady_clock::now();
	// the delta time is the time it took since the beginning of the last frame until now.
	duration<float> deltaTime = duration<float>(nowTime - frameStartTime);
	physicsUpdate += deltaTime;

	// update the delta and frameStartTime
	delta = deltaTime.count();
	frameStartTime = nowTime;
}

void Time::EndOfFrame()
{
	using namespace std::chrono;

	// Update how long it took for the frame to be proccessed.
	frameDuration = steady_clock::now() - frameStartTime;
}

std::chrono::duration<float> Time::GetSleepTime()
{
	// Sleep if the frame was proccessed faster then the wanted frame time.
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
