#include <tracy\Tracy.hpp>
#include <chrono>
#include <map>
#include <string>
#include <any>

#include "Debug\Logger.h"
#include "Debug\StackTrace.h"
#include "DataTypes\Concurrency\Thread.h"
#include "DataTypes\Concurrency\Mutex.h"
#include "DataTypes\Concurrency\Locks.h"
#include "Core\EntryPoint.h"

using namespace Rhygine;
static Logger logger;
static Mutex mutex;

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

static void EndlessThreadLoop()
{
	while (true)
	{
		Logger* log = new Logger();
		ZoneScoped;
		{
			Lock lock = Lock(mutex);
			std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 200));
		}
		logger.Log(Logger::Level::Info, "a");
		std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 200));
		delete log;
	}
}

static void EndlessTest()
{
	ZoneScoped;

	Thread a(EndlessThreadLoop, "A thread");
	Thread b(EndlessThreadLoop);

	a.Start();
	b.Start();

	a.Join();
	b.Join();
}

static void AnyTest()
{
	std::map<std::string, std::any> map;
	{
		std::map<std::string, std::any> sub;
		sub["zero"] = 2;
		sub["one"] = "what";

		map["a"] = sub;
	}

	std::map<std::string, std::any> a = std::any_cast<std::map<std::string, std::any>>(map.at("a"));
	//int zero = std::any_cast<int>(map["a"]["zero"]);
}

int main(int argc, char* argv[])
{
	// EndlessTest();
	EntryPoint::Run(argc, argv);
}
