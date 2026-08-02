#pragma once
#include "IDevice.h"

#include <vulkan/vulkan.h>

namespace Rhygine
{
	class VulkanDevice;
	class VulkanResourceLayout;

	/**
	 * VulkanResourceSet - Vulkan implementation of ResourceSet
	 */
	class VulkanResourceSet : public ResourceSet
	{
	public:
		VulkanResourceSet(VulkanDevice* t_device, const ResourceSetDesc& t_desc);
		~VulkanResourceSet();

		// ResourceSet interface
		[[nodiscard]] ResourceLayout* GetLayout() const override { return m_desc.layout; }
		void UpdateBuffer(uint32_t t_binding, Buffer* t_buffer, uint64_t t_offset = 0, uint64_t t_range = 0) override;
		void UpdateTexture(uint32_t t_binding, Texture* t_texture) override;

		// Vulkan-specific
		[[nodiscard]] VkDescriptorSet GetVkDescriptorSet() const { return m_descriptorSet; }

	private:
		VulkanDevice* m_device;
		VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;
	};
}