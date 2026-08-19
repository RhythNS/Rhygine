#include "RenderGraph.h"

#include <chrono>
#include <cassert>
#include <format>
#include <tracy/Tracy.hpp>

#include "RenderGraphCompiler.h"
#include "RenderGraphResources.h"
#include "TransientResourcePool.h"
#include "Debug/RenderGraphVisualizer.h"
#include "Debug/Logger.h"
#include "IDevice.h"
#include "ICommandBuffer.h"
#include "ResourceManager.h"

namespace Rhygine
{
	RenderGraph::RenderGraph() = default;
	RenderGraph::~RenderGraph() = default;

	RGTextureHandle RenderGraph::Import(const std::string& t_name, Texture* t_texture)
	{
		ZoneScoped;
		LOG_TRACE(std::format("RenderGraph: Importing texture '{}'", t_name));
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
		ZoneScoped;
		LOG_TRACE(std::format("RenderGraph: Importing buffer '{}'", t_name));
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
		ZoneScoped;
		LOG_TRACE(std::format("RenderGraph: Creating virtual texture '{}'", t_name));
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
		ZoneScoped;
		LOG_TRACE(std::format("RenderGraph: Creating virtual buffer '{}'", t_name));
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
		ZoneScoped;
		LOG_DEBUG(std::format("RenderGraph::Compile: Compiling graph with {} passes and {} resources", m_passes.size(), m_resources.size()));
		RenderGraphCompiler::Compile(*this, t_device);
	}

	void RenderGraph::Execute(IDevice* t_device)
	{
		ZoneScoped;
		assert(t_device != nullptr);
		LOG_DEBUG(std::format("RenderGraph::Execute: Executing {} ordered passes", m_executionOrder.size()));

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
		ZoneScoped;
		m_resources.clear();
		m_passes.clear();
		m_executionOrder.clear();
		m_debugInfo = {};
	}

	void RenderGraph::EndFrame()
	{
		ZoneScoped;
		if (m_transientPool)
		{
			m_transientPool->ReleaseAll();
		}
	}

	void RenderGraph::InvalidateTransients()
	{
		ZoneScoped;
		LOG_DEBUG("RenderGraph: Invalidating transient resources");
		m_transientsInvalidated = true;
	}

	void RenderGraph::ExecuteGPUUploads(ResourceManager& t_rm, IDevice& t_device)
	{
		ZoneScoped;
		std::queue<std::unique_ptr<Rhygine::ResourceManager::GPUUploadRequest>> requests = t_rm.DrainStagingRequests();

		if (requests.empty())
		{
			return;
		}

		LOG_DEBUG(std::format("RenderGraph::ExecuteGPUUploads: Processing {} upload requests", requests.size()));
		ICommandBuffer* transferCmd = t_device.GetCommandBuffer(QueueType::Transfer);
		transferCmd->Begin();

		while (!requests.empty())
		{
			std::unique_ptr<Rhygine::ResourceManager::GPUUploadRequest> request = std::move(requests.front());
			requests.pop();

			if (!request || !request->resource)
			{
				continue;
			}

			const auto& stagingReq = request->stagingRequest;
			Resource::StagingResult stagingResult{};

			if (stagingReq.targetType == Resource::StagingRequest::TargetType::Texture)
			{
				Texture* dst = t_device.CreateTexture(stagingReq.textureDesc);

				// Transition dst from Undefined to TransferDst
				TextureBarrier toTransferDst{};
				toTransferDst.texture = dst;
				toTransferDst.srcState = ResourceState::Undefined;
				toTransferDst.dstState = ResourceState::TransferDst;
				toTransferDst.baseMipLevel = 0;
				toTransferDst.levelCount = 0;
				toTransferDst.baseArrayLayer = 0;
				toTransferDst.layerCount = 0;
				transferCmd->ResourceBarrier({}, { toTransferDst });

				// Copy from staging buffer to destination texture
				if (stagingReq.stagingBuffer)
				{
					transferCmd->CopyBufferToTexture(
						stagingReq.stagingBuffer,
						dst,
						stagingReq.textureDesc.width,
						stagingReq.textureDesc.height,
						stagingReq.textureDesc.depth > 0 ? stagingReq.textureDesc.depth : 1
					);
				}

				// Transition dst from TransferDst to ShaderResource
				TextureBarrier toShaderResource{};
				toShaderResource.texture = dst;
				toShaderResource.srcState = ResourceState::TransferDst;
				toShaderResource.dstState = ResourceState::ShaderResource;
				toShaderResource.baseMipLevel = 0;
				toShaderResource.levelCount = 0;
				toShaderResource.baseArrayLayer = 0;
				toShaderResource.layerCount = 0;
				transferCmd->ResourceBarrier({}, { toShaderResource });

				dst->SetState(ResourceState::ShaderResource);

				stagingResult.texture = dst;
				std::string texName = stagingReq.textureDesc.debugName.empty() ? "UploadedTexture" : stagingReq.textureDesc.debugName;
				stagingResult.textureHandle = Import(texName, dst);
			}
			else
			{
				Buffer* dst = t_device.CreateBuffer(stagingReq.bufferDesc);

				// Copy from staging buffer to destination buffer
				if (stagingReq.stagingBuffer)
				{
					uint64_t copySize = stagingReq.dataSize > 0 ? stagingReq.dataSize : stagingReq.bufferDesc.size;
					transferCmd->CopyBuffer(stagingReq.stagingBuffer, dst, copySize);
				}

				// If an initial state was requested, transition from TransferDst to initialState
				if (stagingReq.bufferDesc.initialState != ResourceState::Undefined)
				{
					BufferBarrier toInitial{};
					toInitial.buffer = dst;
					toInitial.srcState = ResourceState::TransferDst;
					toInitial.dstState = stagingReq.bufferDesc.initialState;
					toInitial.offset = 0;
					toInitial.size = 0;
					transferCmd->ResourceBarrier({ toInitial }, {});
					dst->SetState(stagingReq.bufferDesc.initialState);
				}

				stagingResult.buffer = dst;
				std::string bufName = stagingReq.bufferDesc.debugName.empty() ? "UploadedBuffer" : stagingReq.bufferDesc.debugName;
				stagingResult.bufferHandle = Import(bufName, dst);
			}

			// Finalize the resource on the GPU
			request->resource->FinalizeGPU(stagingResult);

			// Stage the staging buffer for deferred deletion
			if (stagingReq.stagingBuffer)
			{
				t_rm.QueueForDeletion(stagingReq.stagingBuffer);
			}

			// Free CPU data if permitted by the resource
			if (request->resource->CanFreeCPUData())
			{
				request->resource->FreeCPUData();
			}
		}

		transferCmd->End();
		t_device.Submit({ transferCmd }, QueueType::Transfer);
	}

	void RenderGraph::ExportGraphviz(const std::string& t_path) const
	{
		ZoneScoped;
		LOG_DEBUG(std::format("RenderGraph: Exporting Graphviz representation to '{}'", t_path));
		RenderGraphVisualizer::ExportGraphviz(*this, t_path);
	}
}
