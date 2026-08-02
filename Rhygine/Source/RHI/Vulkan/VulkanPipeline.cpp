#include "VulkanPipeline.h"

#include <vulkan/vulkan.h>
#include <tracy/Tracy.hpp>

#include "Debug/Error.h"
#include "VulkanDevice.h"
#include "VulkanShader.h"
#include "VulkanResourceLayout.h"
#include "VulkanCheck.h"

namespace Rhygine
{
	VulkanGraphicsPipeline::VulkanGraphicsPipeline(VulkanDevice* t_device, const GraphicsPipelineDesc& t_desc)
		: m_device(t_device)
	{
		ZoneScoped;

		// Shader stages
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		if (t_desc.vertexShader)
		{
			auto* vkShader = static_cast<VulkanShader*>(t_desc.vertexShader);
			VkPipelineShaderStageCreateInfo stageInfo{};
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			stageInfo.module = vkShader->GetVkShaderModule();
			stageInfo.pName = t_desc.vertexShader->GetEntryPoint().c_str();
			shaderStages.push_back(stageInfo);
		}

		if (t_desc.pixelShader)
		{
			auto* vkShader = static_cast<VulkanShader*>(t_desc.pixelShader);
			VkPipelineShaderStageCreateInfo stageInfo{};
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			stageInfo.module = vkShader->GetVkShaderModule();
			stageInfo.pName = t_desc.pixelShader->GetEntryPoint().c_str();
			shaderStages.push_back(stageInfo);
		}

		// Vertex input state
		std::vector<VkVertexInputAttributeDescription> vertexAttributes;
		std::vector<VkVertexInputBindingDescription> vertexBindings;

		for (const auto& attr : t_desc.vertexAttributes)
		{
			VkVertexInputAttributeDescription desc{};
			desc.location = attr.location;
			desc.binding = attr.binding;
			desc.format = t_device->GetVkFormat(attr.format);
			desc.offset = attr.offset;
			vertexAttributes.push_back(desc);
		}

		for (const auto& bind : t_desc.vertexBindings)
		{
			VkVertexInputBindingDescription desc{};
			desc.binding = bind.binding;
			desc.stride = bind.stride;
			desc.inputRate = bind.perInstance ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
			vertexBindings.push_back(desc);
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributes.size());
		vertexInputInfo.pVertexAttributeDescriptions = vertexAttributes.data();
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexBindings.size());
		vertexInputInfo.pVertexBindingDescriptions = vertexBindings.data();

		// Input assembly
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		switch (t_desc.topology)
		{
		case PrimitiveTopology::PointList:
			inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			break;
		case PrimitiveTopology::LineList:
			inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			break;
		case PrimitiveTopology::LineStrip:
			inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			break;
		case PrimitiveTopology::TriangleList:
			inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			break;
		case PrimitiveTopology::TriangleStrip:
			inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			break;
		case PrimitiveTopology::TriangleFan:
			inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
			break;
		case PrimitiveTopology::PatchList:
			inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
			break;
		default:
			inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			break;
		}
		inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		// Viewport state (dynamic)
		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.scissorCount = 1;

		// Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
		rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizerInfo.depthClampEnable = VK_FALSE;
		rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
		switch (t_desc.rasterizerState.fillMode)
		{
		case FillMode::Point:
			rasterizerInfo.polygonMode = VK_POLYGON_MODE_POINT;
			break;
		case FillMode::Wireframe:
			rasterizerInfo.polygonMode = VK_POLYGON_MODE_LINE;
			break;
		case FillMode::Solid:
			rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
			break;
		default:
			rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
			break;
		}
		rasterizerInfo.lineWidth = 1.0f;
		switch (t_desc.rasterizerState.cullMode)
		{
		case CullMode::None:
			rasterizerInfo.cullMode = VK_CULL_MODE_NONE;
			break;
		case CullMode::Front:
			rasterizerInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
			break;
		case CullMode::Back:
			rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
			break;
		default:
			rasterizerInfo.cullMode = VK_CULL_MODE_NONE;
			break;
		}
		rasterizerInfo.frontFace = t_desc.rasterizerState.frontCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
		rasterizerInfo.depthBiasEnable = (t_desc.rasterizerState.depthBias != 0 || t_desc.rasterizerState.slopeScaledDepthBias != 0.0f) ? VK_TRUE : VK_FALSE;
		rasterizerInfo.depthBiasConstantFactor = static_cast<float>(t_desc.rasterizerState.depthBias);
		rasterizerInfo.depthBiasClamp = t_desc.rasterizerState.depthBiasClamp;
		rasterizerInfo.depthBiasSlopeFactor = t_desc.rasterizerState.slopeScaledDepthBias;

		// Multisampling
		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleInfo.sampleShadingEnable = VK_FALSE;
		multisampleInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>(t_desc.sampleCount);

		// Depth Stencil State
		auto MapCompareOp = [](ComparisonFunc func) -> VkCompareOp
			{
				switch (func)
				{
				case ComparisonFunc::Never:
					return VK_COMPARE_OP_NEVER;
				case ComparisonFunc::Less:
					return VK_COMPARE_OP_LESS;
				case ComparisonFunc::Equal:
					return VK_COMPARE_OP_EQUAL;
				case ComparisonFunc::LessEqual:
					return VK_COMPARE_OP_LESS_OR_EQUAL;
				case ComparisonFunc::Greater:
					return VK_COMPARE_OP_GREATER;
				case ComparisonFunc::NotEqual:
					return VK_COMPARE_OP_NOT_EQUAL;
				case ComparisonFunc::GreaterEqual:
					return VK_COMPARE_OP_GREATER_OR_EQUAL;
				case ComparisonFunc::Always:
					return VK_COMPARE_OP_ALWAYS;
				default:
					return VK_COMPARE_OP_LESS;
				}
			};

		auto MapStencilOp = [](StencilOp op) -> VkStencilOp
			{
				switch (op)
				{
				case StencilOp::Keep:
					return VK_STENCIL_OP_KEEP;
				case StencilOp::Zero:
					return VK_STENCIL_OP_ZERO;
				case StencilOp::Replace:
					return VK_STENCIL_OP_REPLACE;
				case StencilOp::IncrementClamp:
					return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
				case StencilOp::DecrementClamp:
					return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
				case StencilOp::Invert:
					return VK_STENCIL_OP_INVERT;
				case StencilOp::IncrementWrap:
					return VK_STENCIL_OP_INCREMENT_AND_WRAP;
				case StencilOp::DecrementWrap:
					return VK_STENCIL_OP_DECREMENT_AND_WRAP;
				default:
					return VK_STENCIL_OP_KEEP;
				}
			};

		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilInfo.depthTestEnable = t_desc.depthStencilState.depthEnable ? VK_TRUE : VK_FALSE;
		depthStencilInfo.depthWriteEnable = t_desc.depthStencilState.depthWriteEnable ? VK_TRUE : VK_FALSE;
		depthStencilInfo.depthCompareOp = MapCompareOp(t_desc.depthStencilState.depthFunc);
		depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		depthStencilInfo.stencilTestEnable = t_desc.depthStencilState.stencilEnable ? VK_TRUE : VK_FALSE;

		depthStencilInfo.front.failOp = MapStencilOp(t_desc.depthStencilState.frontFaceStencilFailOp);
		depthStencilInfo.front.passOp = MapStencilOp(t_desc.depthStencilState.frontFaceStencilPassOp);
		depthStencilInfo.front.depthFailOp = MapStencilOp(t_desc.depthStencilState.frontFaceStencilDepthFailOp);
		depthStencilInfo.front.compareOp = MapCompareOp(t_desc.depthStencilState.frontFaceStencilFunc);
		depthStencilInfo.front.compareMask = t_desc.depthStencilState.stencilReadMask;
		depthStencilInfo.front.writeMask = t_desc.depthStencilState.stencilWriteMask;
		depthStencilInfo.front.reference = 0;

		depthStencilInfo.back.failOp = MapStencilOp(t_desc.depthStencilState.backFaceStencilFailOp);
		depthStencilInfo.back.passOp = MapStencilOp(t_desc.depthStencilState.backFaceStencilPassOp);
		depthStencilInfo.back.depthFailOp = MapStencilOp(t_desc.depthStencilState.backFaceStencilDepthFailOp);
		depthStencilInfo.back.compareOp = MapCompareOp(t_desc.depthStencilState.backFaceStencilFunc);
		depthStencilInfo.back.compareMask = t_desc.depthStencilState.stencilReadMask;
		depthStencilInfo.back.writeMask = t_desc.depthStencilState.stencilWriteMask;
		depthStencilInfo.back.reference = 0;

		// Color blending
		auto MapBlendFactor = [](BlendFactor factor) -> VkBlendFactor
			{
				switch (factor)
				{
				case BlendFactor::Zero:
					return VK_BLEND_FACTOR_ZERO;
				case BlendFactor::One:
					return VK_BLEND_FACTOR_ONE;
				case BlendFactor::SrcColor:
					return VK_BLEND_FACTOR_SRC_COLOR;
				case BlendFactor::InvSrcColor:
					return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
				case BlendFactor::DstColor:
					return VK_BLEND_FACTOR_DST_COLOR;
				case BlendFactor::InvDstColor:
					return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
				case BlendFactor::SrcAlpha:
					return VK_BLEND_FACTOR_SRC_ALPHA;
				case BlendFactor::InvSrcAlpha:
					return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				case BlendFactor::DstAlpha:
					return VK_BLEND_FACTOR_DST_ALPHA;
				case BlendFactor::InvDstAlpha:
					return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
				case BlendFactor::BlendColor:
					return VK_BLEND_FACTOR_CONSTANT_COLOR;
				case BlendFactor::InvBlendColor:
					return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
				case BlendFactor::SrcAlphaSaturate:
					return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
				default:
					return VK_BLEND_FACTOR_ONE;
				}
			};

		auto MapBlendOp = [](BlendOp op) -> VkBlendOp
			{
				switch (op)
				{
				case BlendOp::Add:
					return VK_BLEND_OP_ADD;
				case BlendOp::Subtract:
					return VK_BLEND_OP_SUBTRACT;
				case BlendOp::ReverseSubtract:
					return VK_BLEND_OP_REVERSE_SUBTRACT;
				case BlendOp::Min:
					return VK_BLEND_OP_MIN;
				case BlendOp::Max:
					return VK_BLEND_OP_MAX;
				default:
					return VK_BLEND_OP_ADD;
				}
			};

		std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments(t_desc.renderTargetFormats.size());
		for (auto& attachment : colorBlendAttachments)
		{
			attachment.colorWriteMask = t_desc.blendState.colorWriteMask;
			attachment.blendEnable = t_desc.blendState.enable ? VK_TRUE : VK_FALSE;
			attachment.srcColorBlendFactor = MapBlendFactor(t_desc.blendState.srcColor);
			attachment.dstColorBlendFactor = MapBlendFactor(t_desc.blendState.dstColor);
			attachment.colorBlendOp = MapBlendOp(t_desc.blendState.colorOp);
			attachment.srcAlphaBlendFactor = MapBlendFactor(t_desc.blendState.srcAlpha);
			attachment.dstAlphaBlendFactor = MapBlendFactor(t_desc.blendState.dstAlpha);
			attachment.alphaBlendOp = MapBlendOp(t_desc.blendState.alphaOp);
		}
		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.logicOpEnable = VK_FALSE;
		colorBlendInfo.attachmentCount = static_cast<uint32_t>(colorBlendAttachments.size());
		colorBlendInfo.pAttachments = colorBlendAttachments.data();

		// Dynamic states
		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicStateInfo.pDynamicStates = dynamicStates.data();

		// Pipeline rendering create info (for dynamic rendering)
		VkPipelineRenderingCreateInfo pipelineRenderingInfo{};
		pipelineRenderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		pipelineRenderingInfo.colorAttachmentCount = static_cast<uint32_t>(t_desc.renderTargetFormats.size());
		std::vector<VkFormat> colorFormats;
		for (auto fmt : t_desc.renderTargetFormats)
		{
			colorFormats.push_back(t_device->GetVkFormat(fmt));
		}
		pipelineRenderingInfo.pColorAttachmentFormats = colorFormats.data();
		pipelineRenderingInfo.depthAttachmentFormat = t_device->GetVkFormat(t_desc.depthStencilFormat);

		// Create pipeline layout
		std::vector<VkDescriptorSetLayout> setLayouts;
		for (auto* layout : t_desc.resourceLayouts)
		{
			auto* vkLayout = static_cast<VulkanResourceLayout*>(layout);
			setLayouts.push_back(vkLayout->GetVkDescriptorSetLayout());
		}

		std::vector<VkPushConstantRange> vkPushConstants;
		for (const auto& range : t_desc.pushConstantRanges)
		{
			VkPushConstantRange vkRange{};
			vkRange.stageFlags = t_device->GetVkShaderStageFlags(range.stage);
			vkRange.offset = range.offset;
			vkRange.size = range.size;
			vkPushConstants.push_back(vkRange);
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
		pipelineLayoutInfo.pSetLayouts = setLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(vkPushConstants.size());
		pipelineLayoutInfo.pPushConstantRanges = vkPushConstants.data();

		VK_CHECK(vkCreatePipelineLayout(t_device->GetVkDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout));

		// Create pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
		pipelineInfo.pViewportState = &viewportInfo;
		pipelineInfo.pRasterizationState = &rasterizerInfo;
		pipelineInfo.pMultisampleState = &multisampleInfo;
		pipelineInfo.pDepthStencilState = &depthStencilInfo;
		pipelineInfo.pColorBlendState = &colorBlendInfo;
		pipelineInfo.pDynamicState = &dynamicStateInfo;
		pipelineInfo.pNext = &pipelineRenderingInfo;
		pipelineInfo.layout = m_pipelineLayout;

		VK_CHECK(vkCreateGraphicsPipelines(t_device->GetVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline));

		m_device->SetDebugName(VK_OBJECT_TYPE_PIPELINE, (uint64_t)m_pipeline, t_desc.debugName);
	}

	VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
	{
		ZoneScoped;

		if (m_pipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(m_device->GetVkDevice(), m_pipelineLayout, nullptr);
		}
		if (m_pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(m_device->GetVkDevice(), m_pipeline, nullptr);
		}
	}

	VulkanComputePipeline::VulkanComputePipeline(VulkanDevice* t_device, const ComputePipelineDesc& t_desc)
		: m_device(t_device)
	{
		ZoneScoped;

		auto* vkShader = static_cast<VulkanShader*>(t_desc.computeShader);

		VkPipelineShaderStageCreateInfo stageInfo{};
		stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		stageInfo.module = vkShader->GetVkShaderModule();
		stageInfo.pName = t_desc.computeShader->GetEntryPoint().c_str();

		std::vector<VkDescriptorSetLayout> setLayouts;
		for (auto* layout : t_desc.resourceLayouts)
		{
			auto* vkLayout = static_cast<VulkanResourceLayout*>(layout);
			setLayouts.push_back(vkLayout->GetVkDescriptorSetLayout());
		}

		std::vector<VkPushConstantRange> vkPushConstants;
		for (const auto& range : t_desc.pushConstantRanges)
		{
			VkPushConstantRange vkRange{};
			vkRange.stageFlags = t_device->GetVkShaderStageFlags(range.stage);
			vkRange.offset = range.offset;
			vkRange.size = range.size;
			vkPushConstants.push_back(vkRange);
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
		pipelineLayoutInfo.pSetLayouts = setLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(vkPushConstants.size());
		pipelineLayoutInfo.pPushConstantRanges = vkPushConstants.data();

		VK_CHECK(vkCreatePipelineLayout(t_device->GetVkDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout));

		VkComputePipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineInfo.stage = stageInfo;
		pipelineInfo.layout = m_pipelineLayout;

		VK_CHECK(vkCreateComputePipelines(t_device->GetVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline));

		m_device->SetDebugName(VK_OBJECT_TYPE_PIPELINE, (uint64_t)m_pipeline, t_desc.debugName);
	}

	VulkanComputePipeline::~VulkanComputePipeline()
	{
		ZoneScoped;

		if (m_pipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(m_device->GetVkDevice(), m_pipelineLayout, nullptr);
		}
		if (m_pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(m_device->GetVkDevice(), m_pipeline, nullptr);
		}
	}
}