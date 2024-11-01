#include "EntryPoint.h"

#include <backward.hpp>
#include <vector>
#include <string>
#include <tracy\Tracy.hpp>

#include "Debug\Logger.h"
#include "Debug\Error.h"
#include "Core\MemoryAllocation.h"
#include "Core\Engine.h"

#ifdef _WIN32
#include "Windows\WindowsSystem.h"
#endif

backward::SignalHandling sh;

namespace Rhygine
{
	int EntryPoint::Run(int t_args_count, char* t_args[])
	{
		Logger logger;
		Config config;
		{
			std::vector<std::string> args;
			for (int i = 0; i < t_args_count; i++)
			{
				args.push_back(t_args[i]);
			}

			ParseRuntimeArguments(args, config);
		}

		// null system?

		Engine engine(&config);
		return engine.Run();
	}

#ifdef _WIN32
	int EntryPoint::WinRun(HINSTANCE t_instance, HINSTANCE t_prev_instance, LPSTR t_args, int t_arg_count)
	{
		Logger logger;
		Config config;
		config.Set<bool>("System/AttachConsole", true);
		{
			std::vector<std::string> args;
			for (int i = 0; i < t_arg_count; i++)
			{
			//	args.push_back(t_args[i]);
			}

			ParseRuntimeArguments(args, config);
		}
		WindowsSystem system(t_instance, t_prev_instance, config);

		/*
		if (t_config.TryGet<bool>("System/AttachConsole").value_or(false))
		{

		}
		*/
		system.CreateConsole();
		system.AddWindow();

		while (true)
		{
			std::optional<int> return_code = system.ProcessMessages();
			if (return_code.has_value())
			{
				return return_code.value();
			}
			
			LOG_ERROR("what?");
		}

		Engine engine(&config);
		return engine.Run();
	}
#endif // _WIN32

	inline void EntryPoint::ParseRuntimeArguments(std::vector<std::string>& t_args, Config& t_config)
	{
		ZoneScoped;

		for (size_t i = 0; i < t_args.size(); i++)
		{
			if (t_args[i] == "")
			{

			}
			else
			{
				STOP_EXECUTION_MESSAGE("Unknown runtime argument: " + t_args[i]);
			}
		}
	}
}
