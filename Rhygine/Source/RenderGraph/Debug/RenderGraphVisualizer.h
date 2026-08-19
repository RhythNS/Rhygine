#pragma once

#include <string>

namespace Rhygine
{
	class RenderGraph;

	class RenderGraphVisualizer
	{
	public:
		static void ExportGraphviz(const RenderGraph& t_graph, const std::string& t_path);
	};
}
