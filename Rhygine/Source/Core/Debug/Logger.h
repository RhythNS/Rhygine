#pragma once
#include <string>
#include <memory>

namespace spdlog
{
	class logger;
	namespace level
	{
		enum level_enum : int;
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
			Trace = 0,
			Debug = 1,
			Info = 2,
			Warn = 3,
			Error = 4,

			Off = 10
		};

		Logger();

		void ResetToDefaultLogger() const;

		void Log(const Level t_level, const std::string& t_message) const;
		void Log(const Level t_level, const std::string& t_message, const StackTrace& t_stackTrace) const;
		void Log(const Level t_level, const char* t_file, int t_line, const char* t_function, const std::string& t_message) const;
		void Log(const Level t_level, const char* t_file, int t_line, const char* t_function, const std::string& t_message, const StackTrace& t_stackTrace) const;

		[[nodiscard]] static const Logger* GetInstance();

	private:
		spdlog::level::level_enum FromInternalLevel(const Level t_level) const;

		mutable std::shared_ptr<spdlog::logger> m_logger = nullptr;

		static Logger* s_instance;
	};
}

#ifndef RHY_LOG_LEVEL
#define RHY_LOG_LEVEL 1
#endif 

#define RHY_LOG_LEVEL_TRACE 0
#define RHY_LOG_LEVEL_DEBUG 1
#define RHY_LOG_LEVEL_INFO 2
#define RHY_LOG_LEVEL_WARN 3
#define RHY_LOG_LEVEL_ERROR 4
#define RHY_LOG_LEVEL_OFF 10

#define RHY_LOG(level, ...) Rhygine::Logger::GetInstance()->Log(level, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define RHY_LOG_STACK(level, ...) Rhygine::Logger::GetInstance()->Log(level, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__, Rhygine::StackTrace())

#if RHY_LOG_LEVEL <= RHY_LOG_LEVEL_TRACE
#	define LOG_TRACE(...) RHY_LOG(Rhygine::Logger::Level::Trace, __VA_ARGS__)
#	define LOG_TRACE_STACK(...) RHY_LOG_STACK(Rhygine::Logger::Level::Trace, __VA_ARGS__)
#else
#	define LOG_TRACE(...) (void)0
#	define LOG_TRACE_STACK(...) (void)0
#endif

#if RHY_LOG_LEVEL <= RHY_LOG_LEVEL_DEBUG
#	define LOG_DEBUG(...) RHY_LOG(Rhygine::Logger::Level::Debug, __VA_ARGS__)
#	define LOG_DEBUG_STACK(...) RHY_LOG_STACK(Rhygine::Logger::Level::Debug, __VA_ARGS__)
#else
#	define LOG_DEBUG(...) (void)0
#	define LOG_DEBUG_STACK(...) (void)0
#endif

#if RHY_LOG_LEVEL <= RHY_LOG_LEVEL_INFO
#	define LOG_INFO(...) RHY_LOG(Rhygine::Logger::Level::Info, __VA_ARGS__)
#	define LOG_INFO_STACK(...) RHY_LOG_STACK(Rhygine::Logger::Level::Info, __VA_ARGS__)
#else
#	define LOG_INFO(...) (void)0
#	define LOG_INFO_STACK(...) (void)0
#endif

#if RHY_LOG_LEVEL <= RHY_LOG_LEVEL_WARN
#	define LOG_WARN(...) RHY_LOG(Rhygine::Logger::Level::Warn, __VA_ARGS__)
#	define LOG_WARN_STACK(...) RHY_LOG_STACK(Rhygine::Logger::Level::Warn, __VA_ARGS__)
#else
#	define LOG_WARN(...) (void)0
#	define LOG_WARN_STACK(...) (void)0
#endif

#if RHY_LOG_LEVEL <= RHY_LOG_LEVEL_ERROR
#	define LOG_ERROR(...) RHY_LOG(Rhygine::Logger::Level::Error, __VA_ARGS__)
#	define LOG_ERROR_STACK(...) RHY_LOG_STACK(Rhygine::Logger::Level::Error, __VA_ARGS__)
#else
#	define LOG_ERROR(...) (void)0
#	define LOG_ERROR_STACK(...) (void)0
#endif
