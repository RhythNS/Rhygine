#include "RenderGraph.h"

#include <chrono>
#include <cassert>

#include "RenderGraphCompiler.h"
#include "RenderGraphResources.h"
#include "TransientResourcePool.h"
#include "Debug/RenderGraphVisualizer.h"
#include "IDevice.h"
#include "ICommandBuffer.h"
#include "ResourceManager.h"

namespace Rhygine
{
	RenderGraph::RenderGraph() = default;
	RenderGraph::~RenderGraph() = default;

	RGTextureHandle RenderGraph::Import(const std::string& t_name, Texture* t_texture)
	{
		uint32_t index = static_cast<uint32_t>(m_resources.size());
		RGResourceEntry entry;
		entry.name = t_name;
		entry.isImported = true;
		entry.isTexture = true;
		entry.physicalTexture = t_texture;
		entry.currentState = t_texture ? t_texture->GetState() : ResourceState::Undefined;
		m_resources.push_back(std::move(entry));
		return index;
	}

	RGBufferHandle RenderGraph::Import(const std::string& t_name, Buffer* t_buffer)
	{
		uint32_t index = static_cast<uint32_t>(m_resources.size());
		RGResourceEntry entry;
		entry.name = t_name;
		entry.isImported = true;
		entry.isTexture = false;
		entry.physicalBuffer = t_buffer;
		entry.currentState = t_buffer ? t_buffer->GetState() : ResourceState::Undefined;
		m_resources.push_back(std::move(entry));
		return index;
	}

	RGTextureHandle RenderGraph::CreateTexture(const std::string& t_name, const TextureDesc& t_desc)
	{
		uint32_t index = static_cast<uint32_t>(m_resources.size());
		RGResourceEntry entry;
		entry.name = t_name;
		entry.isImported = false;
		entry.isTexture = true;
		entry.textureDesc = t_desc;
		entry.currentState = ResourceState::Undefined;
		m_resources.push_back(std::move(entry));
		return index;
	}

	RGBufferHandle RenderGraph::CreateBuffer(const std::string& t_name, const BufferDesc& t_desc)
	{
		uint32_t index = static_cast<uint32_t>(m_resources.size());
		RGResourceEntry entry;
		entry.name = t_name;
		entry.isImported = false;
		entry.isTexture = false;
		entry.bufferDesc = t_desc;
		entry.currentState = ResourceState::Undefined;
		m_resources.push_back(std::move(entry));
		return index;
	}

	void RenderGraph::Compile(IDevice* t_device)
	{
		RenderGraphCompiler::Compile(*this, t_device);
	}

	void RenderGraph::Execute(IDevice* t_device)
	{
		assert(t_device != nullptr);

		RenderGraphResources resources(*this);
		ICommandBuffer* cmd = t_device->GetCommandBuffer(QueueType::Graphics);
		cmd->Begin();

		m_debugInfo.passes.clear();
		m_debugInfo.passes.reserve(m_executionOrder.size());

		for (uint32_t execIdx : m_executionOrder)
		{
			RGPassEntry& pass = m_passes[execIdx];
			if (pass.isCulled)
			{
				continue;
			}

			if (!pass.bufferBarriers.empty() || !pass.textureBarriers.empty())
			{
				cmd->ResourceBarrier(pass.bufferBarriers, pass.textureBarriers);
			}

			const auto cbStart = std::chrono::high_resolution_clock::now();
			pass.executeCallback(resources, cmd);
			const auto cbEnd = std::chrono::high_resolution_clock::now();

			PassTimingInfo timingInfo{};
			timingInfo.name = pass.name;
			timingInfo.type = pass.type;
			timingInfo.barrierCount = static_cast<uint32_t>(pass.bufferBarriers.size() + pass.textureBarriers.size());
			timingInfo.cpuRecordTimeMs = std::chrono::duration<float, std::milli>(cbEnd - cbStart).count();
			m_debugInfo.passes.push_back(timingInfo);
		}

		cmd->End();
		m_recordedCommandBuffer = cmd;
	}

	void RenderGraph::BeginFrame()
	{
		m_resources.clear();
		m_passes.clear();
		m_executionOrder.clear();
		m_debugInfo = {};
	}

	void RenderGraph::EndFrame()
	{
		if (m_transientPool)
		{
			m_transientPool->ReleaseAll();
		}
	}

	void RenderGraph::InvalidateTransients()
	{
		m_transientsInvalidated = true;
	}

	void RenderGraph::ExecuteGPUUploads(ResourceManager& t_rm, IDevice& t_device)
	{
		std::queue<std::unique_ptr<Rhygine::ResourceManager::GPUUploadRequest>> requests = t_rm.DrainStagingRequests();

		if (requests.empty())
		{
			return;
		}

		while (!requests.empty())
		{
			std::unique_ptr<Rhygine::ResourceManager::GPUUploadRequest> request = std::move(requests.front());
			requests.pop();

			// TODO: Implement GPU upload logic here.

			request->resource->FinalizeGPU(Resource::StagingResult{});
			// TODO: Maybe have this run in a separate thread to free CPU data
			if (request->resource->CanFreeCPUData())
			{
				request->resource->FreeCPUData();
			}
		}
	}

	void RenderGraph::ExportGraphviz(const std::string& t_path) const
	{
		RenderGraphVisualizer::ExportGraphviz(*this, t_path);
	}
}
