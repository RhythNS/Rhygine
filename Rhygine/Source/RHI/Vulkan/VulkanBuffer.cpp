#include "VulkanBuffer.h"

#include <tracy/Tracy.hpp>

#include "Debug/Error.h"
#include "VulkanDevice.h"
#include "VulkanCheck.h"

namespace Rhygine
{
	VulkanBuffer::VulkanBuffer(VulkanDevice* t_device, const BufferDesc& t_desc)
		: m_device(t_device)
	{
		ZoneScoped;

		m_desc = t_desc;
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = t_desc.size;
		bufferInfo.usage = m_device->GetVkBufferUsageFlags(t_desc.usage);
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocInfo{};
		switch (t_desc.cpuAccess)
		{
		case CpuAccess::Write:
			allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
			allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
			break;
		case CpuAccess::Read:
			allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
			allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
			break;
		case CpuAccess::None:
		default:
			allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
			break;
		}

		VK_CHECK(vmaCreateBuffer(m_device->GetVmaAllocator(), &bufferInfo, &allocInfo, &m_buffer, &m_allocation, nullptr));

		m_currentState = t_desc.initialState;

		m_device->SetDebugName(VK_OBJECT_TYPE_BUFFER, (uint64_t)m_buffer, t_desc.debugName);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		ZoneScoped;

		if (m_buffer != VK_NULL_HANDLE)
		{
			vmaDestroyBuffer(m_device->GetVmaAllocator(), m_buffer, m_allocation);
		}
	}

	void* VulkanBuffer::Map()
	{
		ZoneScoped;

		void* mappedData = nullptr;
		VK_CHECK(vmaMapMemory(m_device->GetVmaAllocator(), m_allocation, &mappedData));
		return mappedData;
	}

	void VulkanBuffer::Unmap()
	{
		ZoneScoped;

		vmaUnmapMemory(m_device->GetVmaAllocator(), m_allocation);
	}
}