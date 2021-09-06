#pragma once
#include <memory>
#include <spdlog/spdlog.h>

class Log
{
public:
	~Log();
	void Init();
	static spdlog::logger* GetLogger();

private:
	static Log* instance;
	std::shared_ptr<spdlog::logger> logger = nullptr;
};

#define RHY_LOG_LEVEL_TRACE 0
#define RHY_LOG_LEVEL_DEBUG 1
#define RHY_LOG_LEVEL_INFO 2
#define RHY_LOG_LEVEL_WARN 3
#define RHY_LOG_LEVEL_ERROR 4
#define RHY_LOG_LEVEL_CRITICAL 5
#define RHY_LOG_LEVEL_OFF 6

#if RHY_LOG_LEVEL <= RHY_LOG_LEVEL_TRACE
#    define LOG_TRACE(...) SPDLOG_LOGGER_CALL(Log::GetLogger(), spdlog::level::trace, __VA_ARGS__)
#else
#    define LOG_TRACE(...) (void)0
#endif

#if RHY_LOG_LEVEL <= RHY_LOG_LEVEL_DEBUG
#    define LOG_DEBUG(...) SPDLOG_LOGGER_CALL(Log::GetLogger(), spdlog::level::debug, __VA_ARGS__)
#else
#    define LOG_DEBUG(...) (void)0
#endif

#if RHY_LOG_LEVEL <= RHY_LOG_LEVEL_INFO
#    define LOG_INFO(...) SPDLOG_LOGGER_CALL(Log::GetLogger(), spdlog::level::info, __VA_ARGS__)
#else
#    define LOG_INFO(...) (void)0
#endif

#if RHY_LOG_LEVEL <= RHY_LOG_LEVEL_WARN
#    define LOG_WARN(...) SPDLOG_LOGGER_CALL(Log::GetLogger(), spdlog::level::warn, __VA_ARGS__)
#else
#    define LOG_WARN(...) (void)0
#endif

#if RHY_LOG_LEVEL <= RHY_LOG_LEVEL_ERROR
#    define LOG_ERROR(...) SPDLOG_LOGGER_CALL(Log::GetLogger(), spdlog::level::err, __VA_ARGS__)
#else
#    define LOG_ERROR(...) (void)0
#endif

#if RHY_LOG_LEVEL <= RHY_LOG_LEVEL_CRITICAL
#    define LOG_CRITICAL(...) SPDLOG_LOGGER_CALL(Log::GetLogger(), spdlog::level::critical, __VA_ARGS__)
#else
#    define LOG_CRITICAL(...) (void)0
#endif
