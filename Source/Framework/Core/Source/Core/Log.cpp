#include <Core/Log.h>
#include <spdlog/sinks/stdout_sinks.h>
#include "spdlog/sinks/stdout_color_sinks.h"

Log::~Log()
{
	if (instance == this)
		instance = nullptr;
}

void Log::Init()
{
	instance = this;
	logger = spdlog::stdout_color_mt("console");
	logger->set_pattern("%^[%X] [%l] [%s:%#]:  %v%$");
	logger->set_level(spdlog::level::trace);
}

spdlog::logger* Log::GetLogger()
{
	return instance->logger.get();
}

Log* Log::instance = nullptr;
