#include "Debug\Logger.h"
#include "Debug\StackTrace.h"

using namespace Rhygine;

static void Test(Logger& p_logger)
{
	Rhygine::StackTrace st;
	p_logger.Log(Logger::Level::Debug, "Stack trace here", st);
}

int main(int argc, char* argv[])
{
	Logger logger;

	Test(logger);

	logger.Log(Logger::Level::Trace, "trace");
	logger.Log(Logger::Level::Debug, "debug");
	logger.Log(Logger::Level::Info, "info");
	logger.Log(Logger::Level::Warn, "warn");
	logger.Log(Logger::Level::Error, "error");

	logger.Log(Logger::Level::Info, "a?");
	logger.Log(Logger::Level::Error, "main.cpp", 10, "main", "a!");
}
