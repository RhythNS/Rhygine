#include <React/ReactLogger.h>
#include <Core/Log.h>

void RhyReact::ReactLogger::log(Level level, const std::string& physicsWorldName, Category category, const std::string& message, const char* filename, int lineNumber)
{
	spdlog::level::level_enum spdLevel = spdlog::level::trace;
	switch (level)
	{
	case reactphysics3d::Logger::Level::Error:
		spdLevel = spdlog::level::err;
		break;
	case reactphysics3d::Logger::Level::Warning:
		spdLevel = spdlog::level::warn;
		break;
	case reactphysics3d::Logger::Level::Information:
		spdLevel = spdlog::level::info;
		break;
	}
	Log::GetLogger()->log(spdLevel, "[" + getCategoryName(category) + "] " + message);
}
