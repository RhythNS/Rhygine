#include "Logger.h"

#include <format>
#include <spdlog\spdlog.h>
#include <spdlog\sinks\stdout_color_sinks.h>
#include <tracy\Tracy.hpp>

#include "StackTrace.h"

Rhygine::Logger::Logger()
{
	std::vector<spdlog::sink_ptr> sinks;

	auto stdSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	stdSink->set_level(spdlog::level::trace);
	sinks.push_back(stdSink);

	auto errSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	errSink->set_level(spdlog::level::err);
	sinks.push_back(errSink);

	m_logger = std::make_shared<spdlog::logger>("main", std::begin(sinks), std::end(sinks));
	m_logger->set_level(spdlog::level::trace);
	m_logger->set_pattern("[%H:%M:%S (%l)%@ %t]: %v");
}

void Rhygine::Logger::Log(const Level t_level, const std::string& t_message) const
{
#ifndef TRACY_NO_LOG
	TracyMessage(t_message.c_str(), t_message.size());
#endif // !TRACY_NO_LOG
	m_logger->log(FromInternalLevel(t_level), t_message);
}

void Rhygine::Logger::Log(const Level t_level, const std::string& t_message, const StackTrace& t_stackTrace) const
{
#ifndef TRACY_NO_LOG
	TracyMessage(t_message.c_str(), t_message.size());
#endif // !TRACY_NO_LOG
	m_logger->log(FromInternalLevel(t_level), "{0}\n{1}", t_message, t_stackTrace.Print());
}

void Rhygine::Logger::Log(const Level t_level, const const char* t_file, int t_line, const const char* t_function, const std::string& t_message) const
{
#ifndef TRACY_NO_LOG
	TracyMessage(t_message.c_str(), t_message.size());
#endif // !TRACY_NO_LOG
	m_logger->log(spdlog::source_loc(t_file, t_line, t_function), FromInternalLevel(t_level), t_message);
}

void Rhygine::Logger::Log(const Level t_level, const char* t_file, int t_line, const char* t_function, const std::string& t_message, const StackTrace& t_stackTrace) const
{
#ifndef TRACY_NO_LOG
	TracyMessage(t_message.c_str(), t_message.size());
#endif // !TRACY_NO_LOG
	m_logger->log(spdlog::source_loc(t_file, t_line, t_function), FromInternalLevel(t_level), "{0}\n{1}", t_message, t_stackTrace.Print());
}

spdlog::level::level_enum Rhygine::Logger::FromInternalLevel(const Level t_level) const
{
	switch (t_level)
	{
	case Level::Trace:
		return spdlog::level::trace;
	case Level::Debug:
		return spdlog::level::debug;
	case Level::Info:
		return spdlog::level::info;
	case Level::Warn:
		return spdlog::level::warn;
	case Level::Error:
		return spdlog::level::err;
	default:
		Log(Level::Error, std::format("Internal log level {} unknown!", static_cast<int>(t_level)));
		return spdlog::level::info;
	}
}
