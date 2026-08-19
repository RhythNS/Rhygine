#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "MaterialCache.h"
#include "DeletionQueue.h"
#include "PassModules/IPassModule.h"
#include "RenderPacket.h"

namespace Rhygine
{
	class IDevice;
	class ISwapchain;
	class RenderGraph;
	struct RenderPacket;
	struct RenderableData;

	struct CameraUBOData
	{
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 viewProj;
		glm::vec3 cameraPos;
		float nearPlane = 0.0f;
		float farPlane = 0.0f;
		float padding[3] = {};
	};

	class SceneRenderer
	{
	public:
		SceneRenderer();
		~SceneRenderer();

		void Init(IDevice* t_device);
		void Shutdown();

		void Render(const RenderPacket& t_packet, RenderGraph& t_graph, ISwapchain* t_swapchain);

		void OnWindowResize(uint32_t t_width, uint32_t t_height);

		void QueueForDeletion(Buffer* t_buffer);
		void QueueForDeletion(Texture* t_texture);

		void RegisterPassModule(std::unique_ptr<IPassModule> t_module);

		MaterialCache& GetMaterialCache() { return m_materialCache; }
		[[nodiscard]] const MaterialCache& GetMaterialCache() const { return m_materialCache; }

		DeletionQueue& GetDeletionQueue() { return m_deletionQueue; }

		[[nodiscard]] Buffer* GetCameraUBO(uint32_t t_frameIndex) const { return m_cameraUBO[t_frameIndex % NUM_FRAMES_IN_FLIGHT]; }
		[[nodiscard]] uint32_t GetCurrentFrameIndex() const { return m_currentFrameIndex; }
		[[nodiscard]] uint32_t GetViewportWidth() const { return m_viewportWidth; }
		[[nodiscard]] uint32_t GetViewportHeight() const { return m_viewportHeight; }
		[[nodiscard]] size_t GetPassModuleCount() const { return m_passModules.size(); }

		void SortRenderables(std::vector<RenderableData>& t_renderables);

	private:
		void UpdateCameraData(const RenderPacket& t_packet);

		static constexpr uint32_t NUM_FRAMES_IN_FLIGHT = 2;
		uint32_t m_currentFrameIndex = 0;
		Buffer* m_cameraUBO[NUM_FRAMES_IN_FLIGHT] = {};

		IDevice* m_device = nullptr;
		MaterialCache m_materialCache;
		DeletionQueue m_deletionQueue;

		std::vector<std::unique_ptr<IPassModule>> m_passModules;

		uint32_t m_viewportWidth  = 0;
		uint32_t m_viewportHeight = 0;
	};
}
