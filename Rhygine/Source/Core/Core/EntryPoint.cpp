#include "EntryPoint.h"

#include <backward.hpp>
#include <vector>
#include <string>
#include <tracy/Tracy.hpp>

#include "Debug/Logger.h"
#include "Debug/Error.h"
#include "Core/MemoryAllocation.h"
#include "DataTypes/Concurrency/Thread.h"
#include "DataTypes/Concurrency/ThreadPool.h"
#include "World.h"
#include "RenderGraph/SceneRenderer.h"
#include "RenderGraph/TripleBufferedPackets.h"
#include "RenderGraph/RenderPacketBuilder.h"
#include "RenderGraph/RenderGraph.h"
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
		
		ThreadPool::Initialize();

		System* system = System::GetInstance();
		system->CreateConsole();
		system->AddWindow();
		Window* window = system->GetWindow(1);

		std::unique_ptr<IDevice> device = IDevice::Create();

		SwapchainDesc swapDesc{};
		swapDesc.window = window;
		swapDesc.width = window->GetWidth();
		swapDesc.height = window->GetHeight();

		ISwapchain* swapchain = device->CreateSwapchain(swapDesc);

		SceneRenderer renderer;
		renderer.Init(device.get());
		RenderPacketBuilder renderPacketBuilder(renderer.GetMaterialCache());
		TripleBufferedPackets tripleBufferedPackets;

		World world(renderer, *device);
		// world.LoadScene();

		RenderGraph renderGraph;

		RenderThread renderThread(renderer, tripleBufferedPackets, renderGraph, swapchain);
		Thread renderThreadHandle(&RenderThread::Run, &renderThread, "RenderThread");
		renderThreadHandle.Start();

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
			
			world.Tick(1.0f / 60.0f);

			world.GetResourceManager().ProcessStagingRequests();

			world.BuildRenderPacket(renderPacketBuilder);
			tripleBufferedPackets.Produce(renderPacketBuilder.Build(renderer.GetCurrentFrameIndex()));

			FrameMark;
		}

		renderThread.RequestStop();
		renderThreadHandle.Join();

		ThreadPool::Shutdown();

		renderer.Shutdown();
		return 0;
	}

	void EntryPoint::RenderThread::Run()
	{
		ZoneScoped;

		while (!m_stopRequested)
		{
			if (m_packets.HasNewPacket())
			{
				m_renderer.Render(m_packets.ConsumeLatest(), m_graph, m_swapchain);
			}
		}
	}
}
