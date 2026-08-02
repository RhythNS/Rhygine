#include "VulkanSwapchain.h"

#include <tracy/Tracy.hpp>

#include "Debug/Error.h"
#include "Systems/Window.h"
#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "VulkanTexture.h"
#include "VulkanCheck.h"

namespace Rhygine
{
	VulkanSwapchain::VulkanSwapchain(VulkanDevice* t_device, const SwapchainDesc& t_desc)
		: m_device(t_device), m_window(t_desc.window), m_desc(t_desc)
	{
		ZoneScoped;

		m_width = t_desc.width;
		m_height = t_desc.height;
		m_format = t_desc.format;

		CreateSurface();
		CreateSwapchain();
		CreateImages();
		CreateSemaphores();
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		ZoneScoped;

		Cleanup();
		DestroySemaphores();

		if (m_surface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(m_device->GetVkInstance(), m_surface, nullptr);
		}
	}

	void VulkanSwapchain::CreateSurface()
	{
		ZoneScoped;

		m_surface = CreateVulkanSurface(m_device->GetVkInstance(), m_window);
	}

	void VulkanSwapchain::CreateSwapchain()
	{
		ZoneScoped;

		// Query surface capabilities
		VkSurfaceCapabilitiesKHR capabilities;
		VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device->GetVkPhysicalDevice(), m_surface, &capabilities));

		// Clamp image count to the surface's supported range.
		uint32_t imageCount = m_desc.minImageCount;
		if (imageCount < capabilities.minImageCount)
		{
			imageCount = capabilities.minImageCount;
		}
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
		{
			imageCount = capabilities.maxImageCount;
		}

		// Clamp image extent to the surface's supported range.
		VkExtent2D extent = { m_width, m_height };
		if (capabilities.currentExtent.width != UINT32_MAX)
		{
			extent = capabilities.currentExtent;
		}
		else
		{
			if (extent.width < capabilities.minImageExtent.width)
				extent.width = capabilities.minImageExtent.width;
			else if (extent.width > capabilities.maxImageExtent.width)
				extent.width = capabilities.maxImageExtent.width;

			if (extent.height < capabilities.minImageExtent.height)
				extent.height = capabilities.minImageExtent.height;
			else if (extent.height > capabilities.maxImageExtent.height)
				extent.height = capabilities.maxImageExtent.height;
		}

		m_width = extent.width;
		m_height = extent.height;

		// Prefer identity transform, otherwise use the current transform.
		VkSurfaceTransformFlagBitsKHR preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		if ((capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) == 0)
		{
			preTransform = capabilities.currentTransform;
		}

		// Pick a supported composite alpha mode.
		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		if ((capabilities.supportedCompositeAlpha & compositeAlpha) == 0)
		{
			if (capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
				compositeAlpha = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
			else if (capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
				compositeAlpha = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
			else if (capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)
				compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
		}

		// Query available present modes
		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_device->GetVkPhysicalDevice(), m_surface, &presentModeCount, nullptr);
		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_device->GetVkPhysicalDevice(), m_surface, &presentModeCount, presentModes.data());

		VkPresentModeKHR desiredMode = VK_PRESENT_MODE_FIFO_KHR; // Always available
		switch (m_desc.presentMode)
		{
		case PresentMode::Mailbox:
			for (auto mode : presentModes)
			{
				if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					desiredMode = mode;
					break;
				}
			}
			break;
		case PresentMode::Immediate:
			for (auto mode : presentModes)
			{
				if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
				{
					desiredMode = mode;
					break;
				}
			}
			break;
		case PresentMode::VSync:
		default:
			break;
		}

		VkSwapchainCreateInfoKHR swapchainInfo{};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = m_surface;
		swapchainInfo.minImageCount = imageCount;
		swapchainInfo.imageFormat = m_device->GetVkFormat(m_format);
		swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swapchainInfo.imageExtent = extent;
		swapchainInfo.imageArrayLayers = m_desc.imageArrayLayers;
		swapchainInfo.imageUsage = m_device->GetVkImageUsageFlags(m_desc.usage);
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.preTransform = preTransform;
		swapchainInfo.compositeAlpha = compositeAlpha;
		swapchainInfo.presentMode = desiredMode;
		swapchainInfo.clipped = VK_TRUE;

		VK_CHECK(vkCreateSwapchainKHR(m_device->GetVkDevice(), &swapchainInfo, nullptr, &m_swapchain));
	}

	void VulkanSwapchain::CreateImages()
	{
		ZoneScoped;

		uint32_t imageCount;
		VK_CHECK(vkGetSwapchainImagesKHR(m_device->GetVkDevice(), m_swapchain, &imageCount, nullptr));
		m_images.resize(imageCount);
		VK_CHECK(vkGetSwapchainImagesKHR(m_device->GetVkDevice(), m_swapchain, &imageCount, m_images.data()));

		m_textures.resize(imageCount);

		TextureDesc textureDesc{};
		textureDesc.width = m_width;
		textureDesc.height = m_height;
		textureDesc.depth = 1;
		textureDesc.mipLevels = 1;
		textureDesc.arrayLayers = 1;
		textureDesc.format = m_format;
		textureDesc.type = TextureType::Texture2D;
		textureDesc.usage = m_desc.usage;
		textureDesc.aspect = TextureAspect::Color;
		textureDesc.initialState = ResourceState::Undefined;

		for (uint32_t i = 0; i < imageCount; i++)
		{
			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = m_images[i];
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = m_device->GetVkFormat(m_format);
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;

			VkImageView imageView = VK_NULL_HANDLE;
			VK_CHECK(vkCreateImageView(m_device->GetVkDevice(), &viewInfo, nullptr, &imageView));

			m_textures[i] = new VulkanTexture(m_device, textureDesc, m_images[i], imageView);
		}
	}

	void VulkanSwapchain::WaitForCurrentFrame()
	{
		ZoneScoped;

		if (!m_inFlightFences.empty() && m_inFlightFences[m_currentFrameIndex] != VK_NULL_HANDLE)
		{
			vkWaitForFences(m_device->GetVkDevice(), 1, &m_inFlightFences[m_currentFrameIndex], VK_TRUE, UINT64_MAX);
			vkResetFences(m_device->GetVkDevice(), 1, &m_inFlightFences[m_currentFrameIndex]);
		}
	}

	void VulkanSwapchain::CreateSemaphores()
	{
		ZoneScoped;

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		const size_t count = m_images.size();
		m_imageAvailableSemaphores.resize(count);
		m_renderFinishedSemaphores.resize(count);

		for (size_t i = 0; i < count; i++)
		{
			VK_CHECK(vkCreateSemaphore(m_device->GetVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]));
			VK_CHECK(vkCreateSemaphore(m_device->GetVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]));
		}

		m_inFlightFences.resize(count);

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < count; i++)
		{
			VK_CHECK(vkCreateFence(m_device->GetVkDevice(), &fenceInfo, nullptr, &m_inFlightFences[i]));
		}
	}

	void VulkanSwapchain::DestroySemaphores()
	{
		ZoneScoped;

		for (size_t i = 0; i < m_imageAvailableSemaphores.size(); i++)
		{
			if (m_imageAvailableSemaphores[i] != VK_NULL_HANDLE)
			{
				vkDestroySemaphore(m_device->GetVkDevice(), m_imageAvailableSemaphores[i], nullptr);
			}
			if (m_renderFinishedSemaphores[i] != VK_NULL_HANDLE)
			{
				vkDestroySemaphore(m_device->GetVkDevice(), m_renderFinishedSemaphores[i], nullptr);
			}
		}
		for (size_t i = 0; i < m_inFlightFences.size(); i++)
		{
			if (m_inFlightFences[i] != VK_NULL_HANDLE)
			{
				vkDestroyFence(m_device->GetVkDevice(), m_inFlightFences[i], nullptr);
			}
		}
		m_imageAvailableSemaphores.clear();
		m_renderFinishedSemaphores.clear();
		m_inFlightFences.clear();
	}

	void VulkanSwapchain::Cleanup()
	{
		ZoneScoped;

		for (auto* texture : m_textures)
		{
			delete texture;
		}
		m_textures.clear();
		m_images.clear();

		if (m_swapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(m_device->GetVkDevice(), m_swapchain, nullptr);
		}
	}

	void VulkanSwapchain::Resize(uint32_t t_width, uint32_t t_height)
	{
		ZoneScoped;

		m_width = t_width;
		m_height = t_height;
		RecreateSwapchain();
	}

	Texture* VulkanSwapchain::GetBackbuffer()
	{
		ZoneScoped;

		return m_textures[m_currentBackbufferIndex];
	}

	uint32_t VulkanSwapchain::AcquireNextImage()
	{
		ZoneScoped;

		VkResult result = vkAcquireNextImageKHR(m_device->GetVkDevice(), m_swapchain, UINT64_MAX, m_imageAvailableSemaphores[m_currentFrameIndex], VK_NULL_HANDLE, &m_currentBackbufferIndex);
		
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapchain();
			result = vkAcquireNextImageKHR(m_device->GetVkDevice(), m_swapchain, UINT64_MAX, m_imageAvailableSemaphores[m_currentFrameIndex], VK_NULL_HANDLE, &m_currentBackbufferIndex);
		}
		
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			STOP_EXECUTION_MESSAGE("Failed to acquire next swapchain image!");
		}

		return m_currentBackbufferIndex;
	}

	void VulkanSwapchain::Present()
	{
		ZoneScoped;

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_renderFinishedSemaphores[m_currentFrameIndex];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_swapchain;
		presentInfo.pImageIndices = &m_currentBackbufferIndex;

		VkResult result = vkQueuePresentKHR(m_device->GetGraphicsQueue(), &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			m_needsRecreation = true;
		}
		else if (result != VK_SUCCESS)
		{
			STOP_EXECUTION_MESSAGE("Failed to present swapchain image!");
		}

		if (!m_images.empty())
		{
			m_currentFrameIndex = (m_currentFrameIndex + 1) % m_images.size();
		}
	}

	void VulkanSwapchain::RecreateSwapchain()
	{
		ZoneScoped;

		// Wait for device to finish all operations
		vkDeviceWaitIdle(m_device->GetVkDevice());

		// Cleanup old swapchain resources & semaphores
		DestroySemaphores();
		Cleanup();

		// Recreate
		CreateSwapchain();
		CreateImages();
		CreateSemaphores();

		m_currentFrameIndex = 0;
		m_needsRecreation = false;
	}

	VkFormat VulkanSwapchain::GetVkFormat() const
	{
		ZoneScoped;

		return m_device->GetVkFormat(m_format);
	}
}