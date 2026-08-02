#pragma once
#include "IDevice.h"

#include <vulkan/vulkan.h>

namespace Rhygine
{
	class VulkanDevice;

	/**
	 * VulkanGraphicsPipeline - Vulkan implementation of graphics Pipeline
	 */
	class VulkanGraphicsPipeline : public Pipeline
	{
	public:
		VulkanGraphicsPipeline(VulkanDevice* t_device, const GraphicsPipelineDesc& t_desc);
		~VulkanGraphicsPipeline();

		// Pipeline interface
		[[nodiscard]] bool IsGraphics() const override { return true; }
		[[nodiscard]] bool IsCompute() const override { return false; }

		// Vulkan-specific
		[[nodiscard]] VkPipeline GetVkPipeline() const { return m_pipeline; }
		[[nodiscard]] VkPipelineLayout GetVkPipelineLayout() const { return m_pipelineLayout; }

	private:
		VulkanDevice* m_device;
		VkPipeline m_pipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	};

	/**
	 * VulkanComputePipeline - Vulkan implementation of compute Pipeline
	 */
	class VulkanComputePipeline : public Pipeline
	{
	public:
		VulkanComputePipeline(VulkanDevice* t_device, const ComputePipelineDesc& t_desc);
		~VulkanComputePipeline();

		// Pipeline interface
		[[nodiscard]] bool IsGraphics() const override { return false; }
		[[nodiscard]] bool IsCompute() const override { return true; }

		// Vulkan-specific
		[[nodiscard]] VkPipeline GetVkPipeline() const { return m_pipeline; }
		[[nodiscard]] VkPipelineLayout GetVkPipelineLayout() const { return m_pipelineLayout; }

	private:
		VulkanDevice* m_device;
		VkPipeline m_pipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	};
}