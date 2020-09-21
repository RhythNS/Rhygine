#include "Time.h"

Time::Time()
{
	lastTime = programStartTime = std::chrono::steady_clock::now();
	delta = 0;
}

float Time::GetDelta()
{
	return delta;
}

float Time::GetTimeSinceStart()
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - programStartTime).count();
}

void Time::Tick()
{
	std::chrono::steady_clock::time_point nowTime = std::chrono::steady_clock::now();
	delta = std::chrono::duration<float>(nowTime - lastTime).count();
	lastTime = nowTime;
}
