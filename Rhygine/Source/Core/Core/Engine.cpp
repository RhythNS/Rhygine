#include "Engine.h"

#include <tracy\Tracy.hpp>

#include "Debug\Logger.h"
#include "Debug\Error.h"

Rhygine::Engine::Engine(Config* t_config)
{
	ZoneScoped;

	LOG_INFO("Rhygine initialized");
}

int Rhygine::Engine::Run()
{
	ZoneScoped;

	return 0;
}
