#include "RenderGraphCompiler.h"
#include "RenderGraph.h"
#include "TransientResourcePool.h"
#include "IDevice.h"

#include <algorithm>
#include <queue>
#include <chrono>
#include <cassert>

namespace Rhygine
{
	void RenderGraphCompiler::Compile(RenderGraph& t_graph, IDevice* t_device)
	{
		const auto startTime = std::chrono::high_resolution_clock::now();

		CullUnusedPasses(t_graph);
		TopologicalSort(t_graph);
		ComputeLifetimes(t_graph);
		AllocateTransients(t_graph, t_device);

		const auto barrierStartTime = std::chrono::high_resolution_clock::now();
		GenerateBarriers(t_graph);
		const auto barrierEndTime = std::chrono::high_resolution_clock::now();

		const auto endTime = std::chrono::high_resolution_clock::now();

		// Populate debug metrics
		t_graph.m_debugInfo.compileTimeMs = std::chrono::duration<float, std::milli>(endTime - startTime).count();
		t_graph.m_debugInfo.barrierGenerationTimeMs = std::chrono::duration<float, std::milli>(barrierEndTime - barrierStartTime).count();

		t_graph.m_debugInfo.transientTextureCount = 0;
		t_graph.m_debugInfo.transientBufferCount = 0;
		t_graph.m_debugInfo.importedResourceCount = 0;
		t_graph.m_debugInfo.resources.clear();

		for (const auto& res : t_graph.m_resources)
		{
			if (res.isImported)
			{
				t_graph.m_debugInfo.importedResourceCount++;
			}
			else if (res.isTexture)
			{
				t_graph.m_debugInfo.transientTextureCount++;
			}
			else
			{
				t_graph.m_debugInfo.transientBufferCount++;
			}

			ResourceLifetimeInfo lifeInfo{};
			lifeInfo.name = res.name;
			lifeInfo.isTransient = !res.isImported;
			lifeInfo.firstPassIndex = res.firstPassIndex;
			lifeInfo.lastPassIndex = res.lastPassIndex;
			t_graph.m_debugInfo.resources.push_back(lifeInfo);
		}
	}

	void RenderGraphCompiler::CullUnusedPasses(RenderGraph& t_graph)
	{
		// Reset refCounts to reflect only direct consumer reads
		for (auto& res : t_graph.m_resources)
		{
			res.refCount = 0;
		}
		for (const auto& pass : t_graph.m_passes)
		{
			for (const auto& read : pass.reads)
			{
				t_graph.m_resources[read.resourceIndex].refCount++;
			}
		}

		// Iterative dead-pass elimination
		bool changed = true;
		while (changed)
		{
			changed = false;
			for (auto& pass : t_graph.m_passes)
			{
				if (pass.isCulled)
				{
					continue;
				}

				if (pass.hasSideEffects)
				{
					continue;
				}

				bool writesImported = false;
				for (const auto& write : pass.writes)
				{
					if (t_graph.m_resources[write.resourceIndex].isImported)
					{
						writesImported = true;
						break;
					}
				}
				if (writesImported)
				{
					continue;
				}

				bool allOutputsUnused = true;
				for (const auto& write : pass.writes)
				{
					if (t_graph.m_resources[write.resourceIndex].refCount > 0)
					{
						allOutputsUnused = false;
						break;
					}
				}

				if (allOutputsUnused)
				{
					pass.isCulled = true;
					changed = true;

					for (const auto& read : pass.reads)
					{
						if (t_graph.m_resources[read.resourceIndex].refCount > 0)
						{
							t_graph.m_resources[read.resourceIndex].refCount--;
						}
					}
				}
			}
		}
	}

	void RenderGraphCompiler::TopologicalSort(RenderGraph& t_graph)
	{
		const size_t numPasses = t_graph.m_passes.size();
		std::vector<std::vector<uint32_t>> adj(numPasses);
		std::vector<uint32_t> inDegree(numPasses, 0);

		auto addEdge = [&](uint32_t u, uint32_t v) {
			if (u == v)
			{
				return;
			}
			if (std::find(adj[u].begin(), adj[u].end(), v) == adj[u].end())
			{
				adj[u].push_back(v);
				inDegree[v]++;
			}
		};

		// Build dependency edges between non-culled passes
		for (uint32_t p = 0; p < numPasses; ++p)
		{
			const auto& passP = t_graph.m_passes[p];
			if (passP.isCulled)
			{
				continue;
			}

			// Read-After-Write (RAW): if P reads R, find non-culled Q that writes R
			for (const auto& read : passP.reads)
			{
				for (uint32_t q = 0; q < numPasses; ++q)
				{
					if (q == p || t_graph.m_passes[q].isCulled)
					{
						continue;
					}

					for (const auto& write : t_graph.m_passes[q].writes)
					{
						if (write.resourceIndex == read.resourceIndex)
						{
							addEdge(q, p);
							break;
						}
					}
				}
			}

			// Write-After-Write (WAW) in declaration order:
			// If P writes R, and Q writes R earlier in declaration order, preserve Q -> P
			for (const auto& writeP : passP.writes)
			{
				for (uint32_t q = 0; q < p; ++q)
				{
					if (t_graph.m_passes[q].isCulled)
					{
						continue;
					}

					for (const auto& writeQ : t_graph.m_passes[q].writes)
					{
						if (writeQ.resourceIndex == writeP.resourceIndex)
						{
							addEdge(q, p);
							break;
						}
					}
				}
			}
		}

		// Kahn's algorithm
		uint32_t nonCulledCount = 0;
		std::queue<uint32_t> queue;

		for (uint32_t i = 0; i < numPasses; ++i)
		{
			if (!t_graph.m_passes[i].isCulled)
			{
				nonCulledCount++;
				if (inDegree[i] == 0)
				{
					queue.push(i);
				}
			}
		}

		t_graph.m_executionOrder.clear();
		t_graph.m_executionOrder.reserve(nonCulledCount);

		while (!queue.empty())
		{
			uint32_t u = queue.front();
			queue.pop();
			t_graph.m_executionOrder.push_back(u);

			for (uint32_t v : adj[u])
			{
				if (--inDegree[v] == 0)
				{
					queue.push(v);
				}
			}
		}

		// Cycle detection fallback
		if (t_graph.m_executionOrder.size() < nonCulledCount)
		{
			// Add any remaining unvisited non-culled passes in declaration order
			for (uint32_t i = 0; i < numPasses; ++i)
			{
				if (!t_graph.m_passes[i].isCulled)
				{
					if (std::find(t_graph.m_executionOrder.begin(), t_graph.m_executionOrder.end(), i) == t_graph.m_executionOrder.end())
					{
						t_graph.m_executionOrder.push_back(i);
					}
				}
			}
		}
	}

	void RenderGraphCompiler::ComputeLifetimes(RenderGraph& t_graph)
	{
		for (auto& res : t_graph.m_resources)
		{
			if (!res.isImported)
			{
				res.firstPassIndex = UINT32_MAX;
				res.lastPassIndex = 0;
			}
		}

		for (uint32_t execIdx = 0; execIdx < static_cast<uint32_t>(t_graph.m_executionOrder.size()); ++execIdx)
		{
			uint32_t passIdx = t_graph.m_executionOrder[execIdx];
			const auto& pass = t_graph.m_passes[passIdx];

			auto recordUsage = [&](uint32_t resIdx) {
				auto& res = t_graph.m_resources[resIdx];
				if (!res.isImported)
				{
					if (res.firstPassIndex == UINT32_MAX)
					{
						res.firstPassIndex = execIdx;
					}
					res.lastPassIndex = execIdx;
				}
			};

			for (const auto& read : pass.reads)
			{
				recordUsage(read.resourceIndex);
			}
			for (const auto& write : pass.writes)
			{
				recordUsage(write.resourceIndex);
			}
		}
	}

	void RenderGraphCompiler::AllocateTransients(RenderGraph& t_graph, IDevice* t_device)
	{
		if (t_graph.m_transientsInvalidated)
		{
			if (t_graph.m_transientPool)
			{
				t_graph.m_transientPool->ReleaseAll();
				t_graph.m_transientPool->GarbageCollect();
			}
			t_graph.m_transientsInvalidated = false;
		}

		if (!t_graph.m_transientPool && t_device)
		{
			t_graph.m_transientPool = std::make_unique<TransientResourcePool>(t_device);
		}

		for (auto& res : t_graph.m_resources)
		{
			if (res.isImported)
			{
				continue;
			}

			// Do not allocate physical memory for resources whose passes were all culled
			if (res.firstPassIndex == UINT32_MAX)
			{
				continue;
			}

			if (t_graph.m_transientPool)
			{
				if (res.isTexture)
				{
					res.physicalTexture = t_graph.m_transientPool->AcquireTexture(res.textureDesc);
				}
				else
				{
					res.physicalBuffer = t_graph.m_transientPool->AcquireBuffer(res.bufferDesc);
				}
			}
		}
	}

	void RenderGraphCompiler::GenerateBarriers(RenderGraph& t_graph)
	{
		for (auto& pass : t_graph.m_passes)
		{
			pass.textureBarriers.clear();
			pass.bufferBarriers.clear();
		}

		for (uint32_t passIdx : t_graph.m_executionOrder)
		{
			auto& pass = t_graph.m_passes[passIdx];

			auto processAccess = [&](const RGPassResourceAccess& access) {
				auto& res = t_graph.m_resources[access.resourceIndex];
				ResourceState srcState = res.currentState;
				ResourceState dstState = access.state;

				if (srcState != dstState)
				{
					if (res.isTexture)
					{
						TextureBarrier barrier{};
						barrier.texture = res.physicalTexture;
						barrier.srcState = srcState;
						barrier.dstState = dstState;
						barrier.baseMipLevel = 0;
						barrier.levelCount = 0;
						barrier.baseArrayLayer = 0;
						barrier.layerCount = 0;
						pass.textureBarriers.push_back(barrier);
					}
					else
					{
						BufferBarrier barrier{};
						barrier.buffer = res.physicalBuffer;
						barrier.srcState = srcState;
						barrier.dstState = dstState;
						barrier.offset = 0;
						barrier.size = 0;
						pass.bufferBarriers.push_back(barrier);
					}
					res.currentState = dstState;
				}
			};

			for (const auto& access : pass.reads)
			{
				processAccess(access);
			}
			for (const auto& access : pass.writes)
			{
				processAccess(access);
			}
		}

		// Imported texture final transition to Present (e.g. swapchain backbuffer)
		for (uint32_t resIdx = 0; resIdx < static_cast<uint32_t>(t_graph.m_resources.size()); ++resIdx)
		{
			auto& res = t_graph.m_resources[resIdx];
			if (res.isImported && res.isTexture)
			{
				if (res.currentState != ResourceState::Present)
				{
					int32_t lastWriterPassIdx = -1;
					for (int32_t i = static_cast<int32_t>(t_graph.m_executionOrder.size()) - 1; i >= 0; --i)
					{
						uint32_t passIdx = t_graph.m_executionOrder[i];
						const auto& pass = t_graph.m_passes[passIdx];
						for (const auto& write : pass.writes)
						{
							if (write.resourceIndex == resIdx)
							{
								lastWriterPassIdx = static_cast<int32_t>(passIdx);
								break;
							}
						}
						if (lastWriterPassIdx != -1)
						{
							break;
						}
					}

					if (lastWriterPassIdx != -1)
					{
						auto& lastPass = t_graph.m_passes[lastWriterPassIdx];
						TextureBarrier barrier{};
						barrier.texture = res.physicalTexture;
						barrier.srcState = res.currentState;
						barrier.dstState = ResourceState::Present;
						barrier.baseMipLevel = 0;
						barrier.levelCount = 0;
						barrier.baseArrayLayer = 0;
						barrier.layerCount = 0;
						lastPass.textureBarriers.push_back(barrier);
						res.currentState = ResourceState::Present;
					}
				}
			}
		}
	}
}
