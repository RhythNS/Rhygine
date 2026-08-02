#pragma once
#include "IDevice.h"

#include <vulkan/vulkan.h>

namespace Rhygine
{
	class VulkanDevice;

	/**
	 * VulkanResourceLayout - Vulkan implementation of ResourceLayout
	 */
	class VulkanResourceLayout : public ResourceLayout
	{
	public:
		VulkanResourceLayout(VulkanDevice* t_device, const ResourceLayoutDesc& t_desc);
		~VulkanResourceLayout();

		// ResourceLayout interface
		[[nodiscard]] const std::vector<ResourceLayoutBinding>& GetBindings() const override { return m_desc.bindings; }

		// Vulkan-specific
		[[nodiscard]] VkDescriptorSetLayout GetVkDescriptorSetLayout() const { return m_descriptorSetLayout; }

	private:
		VulkanDevice* m_device;
		VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
	};
}