#pragma once

#ifdef _WIN32
#include "Windows/RhyWindows.h"
#endif // _WIN32
#include <atomic>
#include "Config/Config.h"

namespace Rhygine
{
	class Config;
	class SceneRenderer;
	class TripleBufferedPackets;
	class RenderGraph;
	class ISwapchain;

	class EntryPoint
	{
	public:
		EntryPoint() = delete;

		static int Run(int t_args_count, char* t_args[]);

#ifdef _WIN32
		static int WinRun(HINSTANCE t_instance, HINSTANCE t_prev_instance, LPSTR t_args, int t_arg_count);
#endif // _WIN32

	private:

		class RenderThread
		{
		public:
			RenderThread(SceneRenderer& t_renderer, TripleBufferedPackets& t_packets, RenderGraph& t_graph, ISwapchain* t_swapchain)
				: m_renderer(t_renderer), m_packets(t_packets), m_graph(t_graph), m_swapchain(t_swapchain)
			{}

			void Run();
			void RequestStop() { m_stopRequested = true; }

		private:
			SceneRenderer& m_renderer;
			TripleBufferedPackets& m_packets;
			RenderGraph& m_graph;
			ISwapchain* m_swapchain = nullptr;
			std::atomic<bool> m_stopRequested = false;
		};


		static inline void ParseRuntimeArguments(std::vector<std::string>& t_args, Config& t_config);

		static inline int Run(Config& t_config);
	};
}
