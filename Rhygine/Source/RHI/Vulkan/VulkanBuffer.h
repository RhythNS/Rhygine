#pragma once
#include "IDevice.h"
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace Rhygine
{
	class VulkanDevice;

	/**
	 * VulkanBuffer - Vulkan implementation of Buffer
	 */
	class VulkanBuffer : public Buffer
	{
	public:
		VulkanBuffer(VulkanDevice* t_device, const BufferDesc& t_desc);
		~VulkanBuffer();

		// Buffer interface
		[[nodiscard]] uint64_t GetSize() const override { return m_desc.size; }
		[[nodiscard]] BufferUsage GetUsage() const override { return m_desc.usage; }
		[[nodiscard]] ResourceState GetState() const override { return m_currentState; }
		void SetState(ResourceState t_state) override { m_currentState = t_state; }
		void* Map() override;
		void Unmap() override;

		// Vulkan-specific
		[[nodiscard]] VkBuffer GetVkBuffer() const { return m_buffer; }
		[[nodiscard]] VmaAllocation GetVmaAllocation() const { return m_allocation; }
		[[nodiscard]] VkDeviceMemory GetVkDeviceMemory() const { return m_memory; }
		[[nodiscard]] VkDeviceSize GetOffset() const { return m_offset; }

	private:
		VulkanDevice* m_device;
		VkBuffer m_buffer = VK_NULL_HANDLE;
		VmaAllocation m_allocation = VK_NULL_HANDLE;
		VkDeviceMemory m_memory = VK_NULL_HANDLE;
		VkDeviceSize m_offset = 0;
	};
}