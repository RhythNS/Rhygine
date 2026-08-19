#include "EntryPoint.h"

#include <backward.hpp>
#include <vector>
#include <string>
#include <tracy/Tracy.hpp>

#include "Debug/Logger.h"
#include "Debug/Error.h"
#include "Core/MemoryAllocation.h"
#include "World.h"
#include "RenderGraph/TempGfx.h"
#include "IDevice.h"

#ifdef _WIN32
#include "Windows/WindowsSystem.h"
#endif

backward::SignalHandling sh;

namespace Rhygine
{
	int EntryPoint::Run(int t_args_count, char* t_args[])
	{
		ZoneScoped;

		Logger logger;
		Config config;
		config.Set<bool>("System/AttachConsole", true);
		{
			std::vector<std::string> args;
			for (int i = 0; i < t_args_count; i++)
			{
				args.push_back(t_args[i]);
			}

			ParseRuntimeArguments(args, config);
		}

		// null system?

		return Run(config);
	}

#ifdef _WIN32
	int EntryPoint::WinRun(HINSTANCE t_instance, HINSTANCE t_prev_instance, LPSTR t_args, int t_arg_count)
	{
		ZoneScoped;

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

		return Run(config);
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
				//STOP_EXECUTION_MESSAGE("Unknown runtime argument: " + t_args[i]);
			}
		}
	}

	inline int EntryPoint::Run(Config& t_config)
	{
		ZoneScoped;

		System* system = System::GetInstance();
		/*
		if (t_config.TryGet<bool>("System/AttachConsole").value_or(false))
		{

		}
		*/
		system->CreateConsole();
		system->AddWindow();

		std::string backend = t_config.GetOr<std::string>("Gfx/Backend", "DX11");

		/*
		if (backend == "DX11")
		{
			STOP_EXECUTION_MESSAGE(backend + " is not available!");
		}
		else
		{
			STOP_EXECUTION_MESSAGE(backend + " not found!");
		}
		*/

		std::unique_ptr<IDevice> device = IDevice::Create();

		World world;
		TempGfx gfx(device.get());

		int exitCode = 0;
		bool running = true;

		while (running)
		{
			ZoneScopedN("Rhygine::EntryPoint::Run::Loop");

			std::optional<int> optExitCode = system->ProcessMessages();
			if (optExitCode.has_value())
			{
				exitCode = optExitCode.value();
				running = false;
			}

			gfx.OnUpdate();

			FrameMark;
		}
		return 0;
	}
}
