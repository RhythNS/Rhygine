#include "VulkanShader.h"

#include <tracy/Tracy.hpp>

#include "Debug/Error.h"
#include "VulkanDevice.h"

namespace Rhygine
{
	VulkanShader::VulkanShader(VulkanDevice* t_device, const ShaderDesc& t_desc)
		: m_device(t_device)
	{
		ZoneScoped;

		m_desc = t_desc;

		VkShaderModuleCreateInfo moduleInfo{};
		moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleInfo.codeSize = t_desc.bytecodeSize;
		moduleInfo.pCode = static_cast<const uint32_t*>(t_desc.bytecode);

		if (vkCreateShaderModule(t_device->GetVkDevice(), &moduleInfo, nullptr, &m_shaderModule) != VK_SUCCESS)
		{
			STOP_EXECUTION_MESSAGE("Failed to create shader module!");
		}

		m_shaderStage = m_device->GetVkShaderStageFlags(t_desc.stage);

		m_device->SetDebugName(VK_OBJECT_TYPE_SHADER_MODULE, (uint64_t)m_shaderModule, t_desc.debugName);
	}

	VulkanShader::~VulkanShader()
	{
		ZoneScoped;

		if (m_shaderModule != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(m_device->GetVkDevice(), m_shaderModule, nullptr);
		}
	}
}