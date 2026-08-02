#pragma once
#include "IDevice.h"

#include <vulkan/vulkan.h>

namespace Rhygine
{
	class VulkanDevice;

	/**
	 * VulkanShader - Vulkan implementation of Shader
	 */
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(VulkanDevice* t_device, const ShaderDesc& t_desc);
		~VulkanShader();

		// Shader interface
		[[nodiscard]] ShaderStage GetStage() const override { return m_desc.stage; }
		[[nodiscard]] const std::string& GetEntryPoint() const override { return m_desc.entryPointName; }

		// Vulkan-specific
		[[nodiscard]] VkShaderModule GetVkShaderModule() const { return m_shaderModule; }
		[[nodiscard]] VkShaderStageFlags GetVkShaderStage() const { return m_shaderStage; }

	private:
		VulkanDevice* m_device;
		VkShaderModule m_shaderModule = VK_NULL_HANDLE;
		VkShaderStageFlags m_shaderStage = VK_SHADER_STAGE_VERTEX_BIT;
	};
}