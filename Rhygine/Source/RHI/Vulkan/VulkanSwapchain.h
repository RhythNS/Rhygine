#pragma once
#include "ISwapchain.h"

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace Rhygine
{
	class VulkanDevice;
	class VulkanTexture;
	class Window;

	/**
	 * VulkanSwapchain - Vulkan implementation of ISwapchain
	 */
	class VulkanSwapchain : public ISwapchain
	{
	public:
		VulkanSwapchain(VulkanDevice* t_device, const SwapchainDesc& t_desc);
		~VulkanSwapchain();

		// ISwapchain interface
		void Resize(uint32_t t_width, uint32_t t_height) override;
		[[nodiscard]] Texture* GetBackbuffer() override;
		[[nodiscard]] uint32_t GetCurrentBackbufferIndex() const override { return m_currentBackbufferIndex; }
		[[nodiscard]] uint32_t GetImageCount() const override { return static_cast<uint32_t>(m_images.size()); }
		[[nodiscard]] uint32_t GetWidth() const override { return m_width; }
		[[nodiscard]] uint32_t GetHeight() const override { return m_height; }
		[[nodiscard]] Format GetFormat() const override { return m_format; }
		[[nodiscard]] uint32_t AcquireNextImage() override;
		void Present() override;
		[[nodiscard]] bool NeedsRecreation() const override { return m_needsRecreation; }
		void MarkRecreated() override { m_needsRecreation = false; }

		void WaitForCurrentFrame() override;
		[[nodiscard]] SyncSemaphore GetImageAvailableSemaphore() override { return { &m_imageAvailableSemaphores[m_currentFrameIndex] }; }
		[[nodiscard]] SyncSemaphore GetRenderFinishedSemaphore() override { return { &m_renderFinishedSemaphores[m_currentFrameIndex] }; }
		[[nodiscard]] SyncFence GetCurrentFence() override { return { &m_inFlightFences[m_currentFrameIndex] }; }

		// Vulkan-specific
		[[nodiscard]] VkFence GetCurrentInFlightFence() const { return m_inFlightFences[m_currentFrameIndex]; }

		// Vulkan-specific
		[[nodiscard]] VkSwapchainKHR GetVkSwapchain() const { return m_swapchain; }
		[[nodiscard]] VkFormat GetVkFormat() const;
		[[nodiscard]] VkSemaphore GetVkImageAvailableSemaphore() const { return m_imageAvailableSemaphores[m_currentFrameIndex]; }
		[[nodiscard]] VkSemaphore GetVkRenderFinishedSemaphore() const { return m_renderFinishedSemaphores[m_currentFrameIndex]; }

	private:
		void CreateSurface();
		void CreateSwapchain();
		void CreateImages();
		void CreateSemaphores();
		void DestroySemaphores();
		void Cleanup();
		void RecreateSwapchain();

		VulkanDevice* m_device;
		Window* m_window;
		SwapchainDesc m_desc;

		VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
		std::vector<VkImage> m_images;
		std::vector<VulkanTexture*> m_textures;

		VkSurfaceKHR m_surface = VK_NULL_HANDLE;

		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_currentBackbufferIndex = 0;
		uint32_t m_currentFrameIndex = 0;
		Format m_format = Format::B8G8R8A8_UNORM;
		bool m_needsRecreation = false;

		std::vector<VkSemaphore> m_imageAvailableSemaphores;
		std::vector<VkSemaphore> m_renderFinishedSemaphores;
		std::vector<VkFence> m_inFlightFences;
	};
}