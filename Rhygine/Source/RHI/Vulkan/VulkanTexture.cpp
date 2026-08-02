#include "VulkanTexture.h"

#include <tracy/Tracy.hpp>

#include "Debug/Error.h"
#include "VulkanDevice.h"
#include "VulkanCheck.h"

namespace Rhygine
{
	VulkanTexture::VulkanTexture(VulkanDevice* t_device, const TextureDesc& t_desc)
		: m_device(t_device)
	{
		ZoneScoped;

		m_desc = t_desc;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = t_desc.width;
		imageInfo.extent.height = t_desc.height;
		imageInfo.extent.depth = t_desc.depth;
		imageInfo.mipLevels = t_desc.mipLevels;
		imageInfo.arrayLayers = t_desc.arrayLayers;
		imageInfo.format = m_device->GetVkFormat(t_desc.format);
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = m_device->GetVkImageUsageFlags(t_desc.usage);
		imageInfo.samples = static_cast<VkSampleCountFlagBits>(t_desc.sampleCount);
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		switch (t_desc.type)
		{
		case TextureType::Texture1D:
			imageInfo.imageType = VK_IMAGE_TYPE_1D;
			break;
		case TextureType::Texture2D:
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			break;
		case TextureType::Texture3D:
			imageInfo.imageType = VK_IMAGE_TYPE_3D;
			break;
		case TextureType::Cube:
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			break;
		}

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

		VK_CHECK(vmaCreateImage(m_device->GetVmaAllocator(), &imageInfo, &allocInfo, &m_image, &m_allocation, nullptr));

		CreateImageView();

		m_currentState = t_desc.initialState;

		m_device->SetDebugName(VK_OBJECT_TYPE_IMAGE, (uint64_t)m_image, t_desc.debugName);
	}

	VulkanTexture::VulkanTexture(VulkanDevice* t_device, const TextureDesc& t_desc, VkImage t_image, VkImageView t_imageView)
		: m_device(t_device), m_image(t_image), m_imageView(t_imageView), m_ownsImage(false), m_ownsImageView(true)
	{
		ZoneScoped;

		m_desc = t_desc;
		m_currentState = t_desc.initialState;
	}

	VulkanTexture::~VulkanTexture()
	{
		ZoneScoped;

		if (m_imageView != VK_NULL_HANDLE && m_ownsImageView)
		{
			vkDestroyImageView(m_device->GetVkDevice(), m_imageView, nullptr);
		}
		if (m_image != VK_NULL_HANDLE && m_ownsImage)
		{
			vmaDestroyImage(m_device->GetVmaAllocator(), m_image, m_allocation);
		}
	}

	void VulkanTexture::CreateImageView()
	{
		ZoneScoped;

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_image;
		switch (m_desc.type)
		{
		case TextureType::Texture1D:
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_1D;
			break;
		case TextureType::Texture2D:
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			break;
		case TextureType::Texture3D:
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
			break;
		case TextureType::Cube:
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
			break;
		default:
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			break;
		}
		viewInfo.format = m_device->GetVkFormat(m_desc.format);
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = m_desc.mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = m_desc.arrayLayers;

		switch (m_desc.aspect)
		{
		case TextureAspect::Color:
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			break;
		case TextureAspect::Depth:
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			break;
		case TextureAspect::Stencil:
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
			break;
		case TextureAspect::DepthStencil:
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
			break;
		}

		VK_CHECK(vkCreateImageView(m_device->GetVkDevice(), &viewInfo, nullptr, &m_imageView));
	}
}