#include "RenderGraphVisualizer.h"

#include <fstream>
#include <unordered_set>

#include "RenderGraph/RenderGraph.h"
#include "RenderGraph/RenderGraphTypes.h"
#include "RHI/RHIDescriptors.h"

namespace Rhygine
{
	namespace
	{
		const char* ResourceStateToString(ResourceState state)
		{
			switch (state)
			{
				case ResourceState::Undefined: return "Undefined";
				case ResourceState::Indirect: return "Indirect";
				case ResourceState::VertexBuffer: return "VertexBuffer";
				case ResourceState::IndexBuffer: return "IndexBuffer";
				case ResourceState::ConstantBuffer: return "ConstantBuffer";
				case ResourceState::ShaderResource: return "ShaderResource";
				case ResourceState::UnorderedAccess: return "UnorderedAccess";
				case ResourceState::RenderTarget: return "RenderTarget";
				case ResourceState::DepthWrite: return "DepthWrite";
				case ResourceState::DepthRead: return "DepthRead";
				case ResourceState::TransferSrc: return "TransferSrc";
				case ResourceState::TransferDst: return "TransferDst";
				case ResourceState::Present: return "Present";
				default: return "Unknown";
			}
		}

		const char* PassTypeToString(RGPassType type)
		{
			switch (type)
			{
				case RGPassType::Graphics: return "Graphics";
				case RGPassType::Compute: return "Compute";
				case RGPassType::Transfer: return "Transfer";
				default: return "Unknown";
			}
		}

		const char* PassTypeFillColor(RGPassType type)
		{
			switch (type)
			{
				case RGPassType::Graphics: return "#D0E8FF"; // Light blue
				case RGPassType::Compute: return "#D0FFD0";  // Light green
				case RGPassType::Transfer: return "#FFF3CD"; // Light yellow
				default: return "#FFFFFF";
			}
		}
	}

	void RenderGraphVisualizer::ExportGraphviz(const RenderGraph& t_graph, const std::string& t_path)
	{
		std::ofstream file(t_path);
		if (!file.is_open())
		{
			return;
		}

		file << "digraph RenderGraph {\n";
		file << "    rankdir=LR;\n";
		file << "    node [shape=box, style=\"filled,rounded\", fontname=\"Helvetica\", fontsize=10];\n";
		file << "    edge [fontname=\"Helvetica\", fontsize=9];\n\n";

		for (uint32_t execIdx : t_graph.m_executionOrder)
		{
			if (execIdx >= t_graph.m_passes.size())
			{
				continue;
			}
			const auto& pass = t_graph.m_passes[execIdx];
			if (pass.isCulled)
			{
				continue;
			}

			file << "    pass_" << execIdx << " [label=\"" << pass.name << "\\n[" << PassTypeToString(pass.type) << "]\", fillcolor=\"" << PassTypeFillColor(pass.type) << "\"];\n";
		}

		bool hasPresentNode = false;
		std::unordered_set<std::string> emittedImportNodes;

		auto findPreviousWriter = [&](size_t currentOrderPosition, uint32_t resIdx) -> int32_t {
			for (int32_t i = static_cast<int32_t>(currentOrderPosition) - 1; i >= 0; --i)
			{
				uint32_t pIdx = t_graph.m_executionOrder[i];
				const auto& prevPass = t_graph.m_passes[pIdx];
				if (prevPass.isCulled)
				{
					continue;
				}

				for (const auto& w : prevPass.writes)
				{
					if (w.resourceIndex == resIdx)
					{
						return static_cast<int32_t>(pIdx);
					}
				}
			}
			return -1;
		};

		for (size_t orderPos = 0; orderPos < t_graph.m_executionOrder.size(); ++orderPos)
		{
			uint32_t execIdx = t_graph.m_executionOrder[orderPos];
			if (execIdx >= t_graph.m_passes.size())
			{
				continue;
			}
			const auto& pass = t_graph.m_passes[execIdx];
			if (pass.isCulled)
			{
				continue;
			}

			for (const auto& barrier : pass.textureBarriers)
			{
				const RGResourceEntry* matchingRes = nullptr;
				uint32_t matchingResIdx = 0;
				for (uint32_t r = 0; r < t_graph.m_resources.size(); ++r)
				{
					if (t_graph.m_resources[r].physicalTexture == barrier.texture)
					{
						matchingRes = &t_graph.m_resources[r];
						matchingResIdx = r;
						break;
					}
				}

				std::string resName = matchingRes ? matchingRes->name : "Texture";

				if (barrier.dstState == ResourceState::Present)
				{
					if (!hasPresentNode)
					{
						file << "\n    Present [shape=ellipse, style=\"filled\", fillcolor=\"#FFE0B2\", label=\"Present\"];\n";
						hasPresentNode = true;
					}
					file << "    pass_" << execIdx << " -> Present [label=\"" << resName << "\\n"
						 << ResourceStateToString(barrier.srcState) << " -> "
						 << ResourceStateToString(barrier.dstState) << "\", color=\"#FF9800\"];\n";
				}
				else
				{
					int32_t writerPassIdx = matchingRes ? findPreviousWriter(orderPos, matchingResIdx) : -1;
					if (writerPassIdx != -1)
					{
						file << "    pass_" << writerPassIdx << " -> pass_" << execIdx << " [label=\"" << resName << "\\n"
							 << ResourceStateToString(barrier.srcState) << " -> "
							 << ResourceStateToString(barrier.dstState) << "\"];\n";
					}
					else if (matchingRes && matchingRes->isImported)
					{
						std::string importNode = "Import_" + resName;
						if (emittedImportNodes.find(importNode) == emittedImportNodes.end())
						{
							file << "    " << importNode << " [shape=note, style=\"filled\", fillcolor=\"#ECEFF1\", label=\"" << resName << "\\n(Imported)\"];\n";
							emittedImportNodes.insert(importNode);
						}
						file << "    " << importNode << " -> pass_" << execIdx << " [label=\""
							 << ResourceStateToString(barrier.srcState) << " -> "
							 << ResourceStateToString(barrier.dstState) << "\", style=dashed];\n";
					}
				}
			}

			for (const auto& barrier : pass.bufferBarriers)
			{
				const RGResourceEntry* matchingRes = nullptr;
				uint32_t matchingResIdx = 0;
				for (uint32_t r = 0; r < t_graph.m_resources.size(); ++r)
				{
					if (t_graph.m_resources[r].physicalBuffer == barrier.buffer)
					{
						matchingRes = &t_graph.m_resources[r];
						matchingResIdx = r;
						break;
					}
				}

				std::string resName = matchingRes ? matchingRes->name : "Buffer";
				int32_t writerPassIdx = matchingRes ? findPreviousWriter(orderPos, matchingResIdx) : -1;
				if (writerPassIdx != -1)
				{
					file << "    pass_" << writerPassIdx << " -> pass_" << execIdx << " [label=\"" << resName << "\\n"
						 << ResourceStateToString(barrier.srcState) << " -> "
						 << ResourceStateToString(barrier.dstState) << "\"];\n";
				}
				else if (matchingRes && matchingRes->isImported)
				{
					std::string importNode = "Import_" + resName;
					if (emittedImportNodes.find(importNode) == emittedImportNodes.end())
					{
						file << "    " << importNode << " [shape=note, style=\"filled\", fillcolor=\"#ECEFF1\", label=\"" << resName << "\\n(Imported)\"];\n";
						emittedImportNodes.insert(importNode);
					}
					file << "    " << importNode << " -> pass_" << execIdx << " [label=\""
						 << ResourceStateToString(barrier.srcState) << " -> "
						 << ResourceStateToString(barrier.dstState) << "\", style=dashed];\n";
				}
			}

			for (const auto& read : pass.reads)
			{
				int32_t writerPassIdx = findPreviousWriter(orderPos, read.resourceIndex);
				if (writerPassIdx != -1)
				{
					const auto& res = t_graph.m_resources[read.resourceIndex];
					bool hadBarrier = false;
					if (res.isTexture)
					{
						for (const auto& b : pass.textureBarriers)
						{
							if (b.texture == res.physicalTexture) { hadBarrier = true; break; }
						}
					}
					else
					{
						for (const auto& b : pass.bufferBarriers)
						{
							if (b.buffer == res.physicalBuffer) { hadBarrier = true; break; }
						}
					}

					if (!hadBarrier)
					{
						file << "    pass_" << writerPassIdx << " -> pass_" << execIdx << " [label=\"" << res.name << "\", style=dotted];\n";
					}
				}
			}
		}

		file << "}\n";
	}
}
