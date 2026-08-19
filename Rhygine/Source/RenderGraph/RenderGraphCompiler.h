#pragma once

#include <vector>
#include <cstdint>

namespace Rhygine
{
	class RenderGraph;
	class IDevice;

	class RenderGraphCompiler
	{
	public:
		static void Compile(RenderGraph& t_graph, IDevice* t_device);

	private:
		static void CullUnusedPasses(RenderGraph& t_graph);

		static void TopologicalSort(RenderGraph& t_graph);

		static void ComputeLifetimes(RenderGraph& t_graph);

		static void AllocateTransients(RenderGraph& t_graph, IDevice* t_device);

		static void GenerateBarriers(RenderGraph& t_graph);
	};
}
