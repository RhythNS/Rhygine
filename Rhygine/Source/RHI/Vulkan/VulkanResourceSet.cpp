#include "VulkanResourceSet.h"

#include <tracy/Tracy.hpp>

#include "Debug/Error.h"
#include "VulkanDevice.h"
#include "VulkanResourceLayout.h"
#include "VulkanBuffer.h"
#include "VulkanTexture.h"
#include "VulkanCheck.h"

#include <unordered_map>

namespace Rhygine
{
	VulkanResourceSet::VulkanResourceSet(VulkanDevice* t_device, const ResourceSetDesc& t_desc)
		: m_device(t_device)
	{
		ZoneScoped;

		m_desc = t_desc;

		auto* vkLayout = static_cast<VulkanResourceLayout*>(t_desc.layout);
		VkDescriptorSetLayout layout = vkLayout ? vkLayout->GetVkDescriptorSetLayout() : VK_NULL_HANDLE;

		m_descriptorSet = m_device->GetDescriptorPoolAllocator()->Allocate(layout);

		m_device->SetDebugName(VK_OBJECT_TYPE_DESCRIPTOR_SET, (uint64_t)m_descriptorSet, t_desc.debugName);
	}

	VulkanResourceSet::~VulkanResourceSet()
	{
		ZoneScoped;

		if (m_descriptorSet != VK_NULL_HANDLE)
		{
			m_device->GetDescriptorPoolAllocator()->Free(m_descriptorSet);
		}
	}

	void VulkanResourceSet::UpdateBuffer(uint32_t t_binding, Buffer* t_buffer, uint64_t t_offset, uint64_t t_range)
	{
		ZoneScoped;

		if (!t_buffer) return;

		auto* vkBuffer = static_cast<VulkanBuffer*>(t_buffer);
		auto* vkLayout = static_cast<VulkanResourceLayout*>(m_desc.layout);

		VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		if (vkLayout)
		{
			for (const auto& b : vkLayout->GetBindings())
			{
				if (b.slot == t_binding)
				{
					if (b.type == ResourceBindingType::Buffer)
						descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
					break;
				}
			}
		}

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = vkBuffer->GetVkBuffer();
		bufferInfo.offset = t_offset;
		bufferInfo.range = (t_range > 0) ? t_range : vkBuffer->GetSize();

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_descriptorSet;
		descriptorWrite.dstBinding = t_binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = descriptorType;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(m_device->GetVkDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	void VulkanResourceSet::UpdateTexture(uint32_t t_binding, Texture* t_texture)
	{
		ZoneScoped;

		if (!t_texture) return;

		auto* vkTexture = static_cast<VulkanTexture*>(t_texture);
		auto* vkLayout = static_cast<VulkanResourceLayout*>(m_desc.layout);

		VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		if (vkLayout)
		{
			for (const auto& b : vkLayout->GetBindings())
			{
				if (b.slot == t_binding)
				{
					if (b.type == ResourceBindingType::StorageTexture)
						descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
					break;
				}
			}
		}

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = (descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE)
			? VK_IMAGE_LAYOUT_GENERAL
			: VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = vkTexture->GetVkImageView();
		imageInfo.sampler = (descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) ? m_device->GetVkDefaultSampler() : VK_NULL_HANDLE;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_descriptorSet;
		descriptorWrite.dstBinding = t_binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = descriptorType;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(m_device->GetVkDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}