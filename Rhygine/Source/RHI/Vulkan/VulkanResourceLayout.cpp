#include "VulkanResourceLayout.h"

#include <tracy/Tracy.hpp>

#include "Debug/Error.h"
#include "VulkanDevice.h"
#include "VulkanCheck.h"

namespace Rhygine
{
	VulkanResourceLayout::VulkanResourceLayout(VulkanDevice* t_device, const ResourceLayoutDesc& t_desc)
		: m_device(t_device)
	{
		ZoneScoped;

		m_desc = t_desc;

		std::vector<VkDescriptorSetLayoutBinding> bindings;

		for (const auto& binding : t_desc.bindings)
		{
			VkDescriptorSetLayoutBinding vkBinding{};
			vkBinding.binding = binding.slot;
			vkBinding.descriptorCount = binding.count;

			switch (binding.type)
			{
			case ResourceBindingType::ConstantBuffer:
				vkBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				break;
			case ResourceBindingType::Texture:
				vkBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				break;
			case ResourceBindingType::StorageTexture:
				vkBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				break;
			case ResourceBindingType::Buffer:
				vkBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				break;
			case ResourceBindingType::Sampler:
				vkBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
				break;
			}

			vkBinding.stageFlags = m_device->GetVkShaderStageFlags(binding.shaderStage);
			bindings.push_back(vkBinding);
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		VK_CHECK(vkCreateDescriptorSetLayout(t_device->GetVkDevice(), &layoutInfo, nullptr, &m_descriptorSetLayout));

		m_device->SetDebugName(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, (uint64_t)m_descriptorSetLayout, t_desc.debugName);
	}

	VulkanResourceLayout::~VulkanResourceLayout()
	{
		ZoneScoped;

		if (m_descriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(m_device->GetVkDevice(), m_descriptorSetLayout, nullptr);
		}
	}
}