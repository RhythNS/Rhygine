#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "IDevice.h"

namespace Rhygine
{
	class VulkanDevice;

	/**
	 * VulkanTexture - Vulkan implementation of Texture
	 */
	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(VulkanDevice* t_device, const TextureDesc& t_desc);
		VulkanTexture(VulkanDevice* t_device, const TextureDesc& t_desc, VkImage t_image, VkImageView t_imageView);

		~VulkanTexture();

		// Texture interface
		[[nodiscard]] uint32_t GetWidth() const override { return m_desc.width; }
		[[nodiscard]] uint32_t GetHeight() const override { return m_desc.height; }
		[[nodiscard]] uint32_t GetDepth() const override { return m_desc.depth; }
		[[nodiscard]] uint32_t GetMipLevels() const override { return m_desc.mipLevels; }
		[[nodiscard]] Format GetFormat() const override { return m_desc.format; }
		[[nodiscard]] TextureType GetType() const override { return m_desc.type; }
		[[nodiscard]] TextureUsage GetUsage() const override { return m_desc.usage; }
		[[nodiscard]] TextureAspect GetAspect() const override { return m_desc.aspect; }
		[[nodiscard]] ResourceState GetState() const override { return m_currentState; }
		void SetState(ResourceState t_state) override { m_currentState = t_state; }

		// Vulkan-specific
		[[nodiscard]] VkImage GetVkImage() const { return m_image; }
		[[nodiscard]] VkImageView GetVkImageView() const { return m_imageView; }
		[[nodiscard]] VmaAllocation GetVmaAllocation() const { return m_allocation; }

	private:
		void CreateImageView();

		VulkanDevice* m_device;
		VkImage m_image = VK_NULL_HANDLE;
		VkImageView m_imageView = VK_NULL_HANDLE;
		VmaAllocation m_allocation = VK_NULL_HANDLE;
		bool m_ownsImage = true;
		bool m_ownsImageView = true;
	};
}