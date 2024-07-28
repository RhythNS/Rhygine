#include <tracy\Tracy.hpp>
#include <chrono>

#include "Debug\Logger.h"
#include "Debug\StackTrace.h"
#include "DataTypes\Concurrency\Thread.h"

using namespace Rhygine;
static Logger logger;

static void LogStackTrace()
{
	Rhygine::StackTrace st;
	logger.Log(Logger::Level::Debug, "Stack trace here", st);
}

static void LogOnAllLevels()
{
	logger.Log(Logger::Level::Trace, "trace");
	logger.Log(Logger::Level::Debug, "debug");
	logger.Log(Logger::Level::Info, "info");
	logger.Log(Logger::Level::Warn, "warn");
	logger.Log(Logger::Level::Error, "error");

	logger.Log(Logger::Level::Info, "a?");
	logger.Log(Logger::Level::Error, "main.cpp", 10, "main", "a!");
}

void EndlessThreadLoop()
{
	while (true)
	{
		ZoneScoped;
		std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 200));
		logger.Log(Logger::Level::Info, "a");
	}
}

int main(int argc, char* argv[])
{
	ZoneScoped;

	Thread a(EndlessThreadLoop, "A thread");
	Thread b(EndlessThreadLoop);

	a.Start();
	b.Start();

	a.Join();
	b.Join();
}
