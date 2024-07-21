#pragma once
#include <string>
#include <memory>

namespace spdlog
{
	class logger;
	namespace level
	{
		enum level_enum;
	}
}

namespace Rhygine {
	class StackTrace;
}

namespace Rhygine {
	class Logger
	{
	public:
		enum class Level {
			Trace,
			Debug,
			Info,
			Warn,
			Error
		};

		Logger();

		void Log(const Level t_level, const std::string& t_message) const;
		void Log(const Level t_level, const std::string& t_message, const StackTrace& t_stackTrace) const;
		void Log(const Level t_level, const char* t_file, int t_line, const char* t_function, const std::string& t_message) const;
		void Log(const Level t_level, const char* t_file, int t_line, const char* t_function, const std::string& t_message, const StackTrace& t_stackTrace) const;

	private:
		spdlog::level::level_enum FromInternalLevel(const Level t_level) const;

		std::shared_ptr<spdlog::logger> m_logger = nullptr;
	};
}
