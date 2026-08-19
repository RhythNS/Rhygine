#include "SceneRenderer.h"

#include <algorithm>
#include <cstring>

#include "IDevice.h"
#include "ISwapchain.h"
#include "RenderGraph.h"

namespace Rhygine
{
	SceneRenderer::SceneRenderer()
		: m_materialCache(nullptr)
	{
		for (uint32_t i = 0; i < NUM_FRAMES_IN_FLIGHT; ++i)
		{
			m_cameraUBO[i] = nullptr;
		}
	}

	SceneRenderer::~SceneRenderer()
	{
		Shutdown();
	}

	void SceneRenderer::Init(IDevice* t_device)
	{
		Shutdown();

		m_device = t_device;
		m_materialCache.SetDevice(t_device);

		if (m_device)
		{
			BufferDesc uboDesc{};
			uboDesc.size = sizeof(CameraUBOData);
			uboDesc.usage = BufferUsage::Constant;
			uboDesc.cpuAccess = CpuAccess::Write;
			uboDesc.initialState = ResourceState::ConstantBuffer;
			uboDesc.debugName = "CameraUBO";

			for (uint32_t i = 0; i < NUM_FRAMES_IN_FLIGHT; ++i)
			{
				m_cameraUBO[i] = m_device->CreateBuffer(uboDesc);
			}
		}
	}

	void SceneRenderer::Shutdown()
	{
		if (m_device)
		{
			for (uint32_t i = 0; i < NUM_FRAMES_IN_FLIGHT; ++i)
			{
				if (m_cameraUBO[i])
				{
					m_device->DestroyBuffer(m_cameraUBO[i]);
					m_cameraUBO[i] = nullptr;
				}
			}
		}

		m_passModules.clear();
		m_materialCache.InvalidateAll();
		m_device = nullptr;
	}

	void SceneRenderer::OnWindowResize(uint32_t t_width, uint32_t t_height)
	{
		m_viewportWidth = t_width;
		m_viewportHeight = t_height;
	}

	void SceneRenderer::QueueForDeletion(Buffer* t_buffer)
	{
		m_deletionQueue.Push(t_buffer, m_currentFrameIndex);
	}

	void SceneRenderer::QueueForDeletion(Texture* t_texture)
	{
		m_deletionQueue.Push(t_texture, m_currentFrameIndex);
	}

	void SceneRenderer::RegisterPassModule(std::unique_ptr<IPassModule> t_module)
	{
		if (t_module)
		{
			m_passModules.push_back(std::move(t_module));
		}
	}

	void SceneRenderer::SortRenderables(std::vector<RenderableData>& t_renderables)
	{
		std::sort(t_renderables.begin(), t_renderables.end(), [](const RenderableData& a, const RenderableData& b) {
			return a.sortKey < b.sortKey;
		});
	}

	void SceneRenderer::UpdateCameraData(const RenderPacket& t_packet)
	{
		CameraUBOData data{};
		data.view = t_packet.viewMatrix;
		data.projection = t_packet.projectionMatrix;
		data.viewProj = t_packet.projectionMatrix * t_packet.viewMatrix;
		data.cameraPos = t_packet.cameraPosition;
		data.nearPlane = t_packet.nearPlane;
		data.farPlane = t_packet.farPlane;

		Buffer* ubo = m_cameraUBO[m_currentFrameIndex % NUM_FRAMES_IN_FLIGHT];
		if (ubo)
		{
			void* mapped = ubo->Map();
			if (mapped)
			{
				std::memcpy(mapped, &data, sizeof(data));
				ubo->Unmap();
			}
		}
	}

	void SceneRenderer::Render(const RenderPacket& t_packet, RenderGraph& t_graph, ISwapchain* t_swapchain)
	{
		t_graph.BeginFrame();

		UpdateCameraData(t_packet);

		RGTextureHandle backbuffer = RG_INVALID_TEXTURE;
		if (t_swapchain && t_swapchain->GetBackbuffer())
		{
			backbuffer = t_graph.Import("Backbuffer", t_swapchain->GetBackbuffer());
		}

		uint32_t width = m_viewportWidth;
		uint32_t height = m_viewportHeight;
		if ((width == 0 || height == 0) && t_swapchain)
		{
			width = t_swapchain->GetWidth();
			height = t_swapchain->GetHeight();
		}
		if (width == 0) width = 1;
		if (height == 0) height = 1;

		TextureDesc depthDesc{};
		depthDesc.width = width;
		depthDesc.height = height;
		depthDesc.depth = 1;
		depthDesc.mipLevels = 1;
		depthDesc.arrayLayers = 1;
		depthDesc.format = Format::D32_SFLOAT;
		depthDesc.usage = TextureUsage::DepthStencil | TextureUsage::Sampled;
		depthDesc.type = TextureType::Texture2D;
		depthDesc.sampleCount = SampleCount::Count1;
		depthDesc.initialState = ResourceState::Undefined;
		depthDesc.debugName = "SceneDepth";

		RGTextureHandle sceneDepth = t_graph.CreateTexture("SceneDepth", depthDesc);

		FrameResources frameRes{ backbuffer, sceneDepth, RG_INVALID_BUFFER };

		for (auto& module : m_passModules)
		{
			module->AddPasses(t_graph, t_packet, frameRes);
		}

		t_graph.Compile(m_device);
		t_graph.Execute(m_device);

		if (t_swapchain && m_device)
		{
			ICommandBuffer* cmd = t_graph.GetRecordedCommandBuffer();
			if (cmd)
			{
				m_device->Submit(
					{ cmd },
					t_swapchain->GetImageAvailableSemaphore(),
					t_swapchain->GetRenderFinishedSemaphore()
				);
			}

			t_swapchain->Present();
		}

		t_graph.EndFrame();

		if (m_device)
		{
			m_deletionQueue.Flush(m_currentFrameIndex, m_device);
		}

		m_currentFrameIndex++;
	}
}
