#pragma once
#include <reactphysics3d/utils/Logger.h>

namespace RhyReact
{
	class ReactLogger : public reactphysics3d::Logger
	{
	public:
		virtual void log(Level level, const std::string& physicsWorldName, Category category, const std::string& message, const char* filename, int lineNumber) override;
	};
}
