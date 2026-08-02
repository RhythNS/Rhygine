#include "VulkanCommandBuffer.h"

#include <tracy/Tracy.hpp>

#include "VulkanDevice.h"
#include "VulkanPipeline.h"
#include "VulkanResourceSet.h"
#include "VulkanBuffer.h"
#include "VulkanTexture.h"
#include "VulkanCheck.h"

namespace Rhygine
{
	VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* t_device, uint32_t t_queueFamilyIndex)
		: m_device(t_device)
	{
		ZoneScoped;

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = t_queueFamilyIndex;

		VK_CHECK(vkCreateCommandPool(t_device->GetVkDevice(), &poolInfo, nullptr, &m_commandPool));

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		VK_CHECK(vkAllocateCommandBuffers(t_device->GetVkDevice(), &allocInfo, &m_commandBuffer));
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		ZoneScoped;

		if (m_commandBuffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers(m_device->GetVkDevice(), m_commandPool, 1, &m_commandBuffer);
		}
		if (m_commandPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(m_device->GetVkDevice(), m_commandPool, nullptr);
		}
	}

	void VulkanCommandBuffer::Begin()
	{
		ZoneScoped;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		VK_CHECK(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
	}

	void VulkanCommandBuffer::End()
	{
		ZoneScoped;

		VK_CHECK(vkEndCommandBuffer(m_commandBuffer));
	}

	void VulkanCommandBuffer::BindPipeline(Pipeline* t_pipeline)
	{
		ZoneScoped;

		if (t_pipeline->IsGraphics())
		{
			auto* vkPipeline = static_cast<VulkanGraphicsPipeline*>(t_pipeline);
			vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline->GetVkPipeline());
			m_pipelineLayout = vkPipeline->GetVkPipelineLayout();
			m_isComputePipeline = false;
		}
		else
		{
			auto* vkPipeline = static_cast<VulkanComputePipeline*>(t_pipeline);
			vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, vkPipeline->GetVkPipeline());
			m_pipelineLayout = vkPipeline->GetVkPipelineLayout();
			m_isComputePipeline = true;
		}
	}

	void VulkanCommandBuffer::BindResourceSet(uint32_t t_slot, ResourceSet* t_set)
	{
		ZoneScoped;

		if (m_pipelineLayout == VK_NULL_HANDLE)
		{
			STOP_EXECUTION_MESSAGE("Cannot bind resource set before binding a valid pipeline!");
		}

		auto* vkSet = static_cast<VulkanResourceSet*>(t_set);
		VkDescriptorSet descriptorSet = vkSet->GetVkDescriptorSet();
		VkPipelineBindPoint bindPoint = m_isComputePipeline ? VK_PIPELINE_BIND_POINT_COMPUTE : VK_PIPELINE_BIND_POINT_GRAPHICS;
		vkCmdBindDescriptorSets(m_commandBuffer, bindPoint, m_pipelineLayout, t_slot, 1, &descriptorSet, 0, nullptr);
	}

	void VulkanCommandBuffer::BindVertexBuffer(uint32_t t_slot, Buffer* t_buffer, uint64_t t_offset)
	{
		ZoneScoped;

		auto* vkBuffer = static_cast<VulkanBuffer*>(t_buffer);
		VkBuffer buffers[] = { vkBuffer->GetVkBuffer() };
		VkDeviceSize offsets[] = { t_offset };
		vkCmdBindVertexBuffers(m_commandBuffer, t_slot, 1, buffers, offsets);
	}

	void VulkanCommandBuffer::BindIndexBuffer(Buffer* t_buffer, uint64_t t_offset, IndexFormat t_format)
	{
		ZoneScoped;

		auto* vkBuffer = static_cast<VulkanBuffer*>(t_buffer);
		VkIndexType indexType = (t_format == IndexFormat::Uint32) ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16;
		vkCmdBindIndexBuffer(m_commandBuffer, vkBuffer->GetVkBuffer(), t_offset, indexType);
	}

	void VulkanCommandBuffer::SetViewport(const Viewport& t_viewport)
	{
		ZoneScoped;

		VkViewport viewport{};
		viewport.x = t_viewport.x;
		viewport.y = t_viewport.y;
		viewport.width = t_viewport.width;
		viewport.height = t_viewport.height;
		viewport.minDepth = t_viewport.minDepth;
		viewport.maxDepth = t_viewport.maxDepth;
		vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);
	}

	void VulkanCommandBuffer::SetScissor(const ScissorRect& t_scissor)
	{
		ZoneScoped;

		VkRect2D scissor{};
		scissor.offset = { t_scissor.x, t_scissor.y };
		scissor.extent = { t_scissor.width, t_scissor.height };
		vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
	}

	static VkAttachmentLoadOp GetVkAttachmentLoadOp(AttachmentLoadOp op)
	{
		switch (op)
		{
		case AttachmentLoadOp::Load:
			return VK_ATTACHMENT_LOAD_OP_LOAD;
		case AttachmentLoadOp::Clear:
			return VK_ATTACHMENT_LOAD_OP_CLEAR;
		case AttachmentLoadOp::DontCare:
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		default:
			return VK_ATTACHMENT_LOAD_OP_CLEAR;
		}
	}

	void VulkanCommandBuffer::BeginRenderPass(const RenderPassBeginInfo& t_info)
	{
		ZoneScoped;

		std::vector<VkRenderingAttachmentInfo> colorAttachments;
		VkRenderingAttachmentInfo depthAttachment{};

		VkRenderingInfo renderingInfo{};
		renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
		renderingInfo.layerCount = 1;
		renderingInfo.renderArea.extent.width = t_info.width;
		renderingInfo.renderArea.extent.height = t_info.height;
		renderingInfo.renderArea.offset = { 0, 0 };

		// Color attachments
		if (!t_info.colorTargets.empty())
		{
			colorAttachments.resize(t_info.colorTargets.size());
			for (size_t i = 0; i < t_info.colorTargets.size(); i++)
			{
				auto* vkTexture = static_cast<VulkanTexture*>(t_info.colorTargets[i]);
				VkClearValue clearValue{};
				if (i < t_info.clearValues.size() && !t_info.clearValues[i].isDepthStencil)
				{
					clearValue.color.float32[0] = t_info.clearValues[i].color.r;
					clearValue.color.float32[1] = t_info.clearValues[i].color.g;
					clearValue.color.float32[2] = t_info.clearValues[i].color.b;
					clearValue.color.float32[3] = t_info.clearValues[i].color.a;
				}

				VkAttachmentLoadOp loadOp = (i < t_info.colorLoadOps.size())
					? GetVkAttachmentLoadOp(t_info.colorLoadOps[i])
					: VK_ATTACHMENT_LOAD_OP_CLEAR;

				colorAttachments[i].sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
				colorAttachments[i].imageView = vkTexture->GetVkImageView();
				colorAttachments[i].imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				colorAttachments[i].loadOp = loadOp;
				colorAttachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachments[i].clearValue = clearValue;
			}
			renderingInfo.colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size());
			renderingInfo.pColorAttachments = colorAttachments.data();
		}

		// Depth attachment
		if (t_info.depthTarget != nullptr)
		{
			auto* vkTexture = static_cast<VulkanTexture*>(t_info.depthTarget);
			VkClearValue clearValue{};
			if (!t_info.clearValues.empty() && t_info.clearValues.back().isDepthStencil)
			{
				clearValue.depthStencil.depth = t_info.clearValues.back().depthStencil.depth;
				clearValue.depthStencil.stencil = t_info.clearValues.back().depthStencil.stencil;
			}

			depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			depthAttachment.imageView = vkTexture->GetVkImageView();
			depthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			depthAttachment.loadOp = GetVkAttachmentLoadOp(t_info.depthLoadOp);
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			depthAttachment.clearValue = clearValue;
			renderingInfo.pDepthAttachment = &depthAttachment;
		}

		vkCmdBeginRendering(m_commandBuffer, &renderingInfo);
	}

	void VulkanCommandBuffer::EndRenderPass()
	{
		ZoneScoped;

		vkCmdEndRendering(m_commandBuffer);
	}

	void VulkanCommandBuffer::Draw(uint32_t t_vertexCount, uint32_t t_instanceCount, uint32_t t_firstVertex, uint32_t t_firstInstance)
	{
		ZoneScoped;

		vkCmdDraw(m_commandBuffer, t_vertexCount, t_instanceCount, t_firstVertex, t_firstInstance);
	}

	void VulkanCommandBuffer::DrawIndexed(uint32_t t_indexCount, uint32_t t_instanceCount, uint32_t t_firstIndex, int32_t t_vertexOffset, uint32_t t_firstInstance)
	{
		ZoneScoped;

		vkCmdDrawIndexed(m_commandBuffer, t_indexCount, t_instanceCount, t_firstIndex, t_vertexOffset, t_firstInstance);
	}

	void VulkanCommandBuffer::DrawIndirect(Buffer* t_buffer, uint64_t t_offset, uint32_t t_drawCount, uint32_t t_stride)
	{
		ZoneScoped;

		auto* vkBuffer = static_cast<VulkanBuffer*>(t_buffer);
		vkCmdDrawIndirect(m_commandBuffer, vkBuffer->GetVkBuffer(), t_offset, t_drawCount, t_stride);
	}

	void VulkanCommandBuffer::DrawIndexedIndirect(Buffer* t_buffer, uint64_t t_offset, uint32_t t_drawCount, uint32_t t_stride)
	{
		ZoneScoped;

		auto* vkBuffer = static_cast<VulkanBuffer*>(t_buffer);
		vkCmdDrawIndexedIndirect(m_commandBuffer, vkBuffer->GetVkBuffer(), t_offset, t_drawCount, t_stride);
	}

	void VulkanCommandBuffer::Dispatch(uint32_t t_groupCountX, uint32_t t_groupCountY, uint32_t t_groupCountZ)
	{
		ZoneScoped;

		vkCmdDispatch(m_commandBuffer, t_groupCountX, t_groupCountY, t_groupCountZ);
	}

	void VulkanCommandBuffer::DispatchIndirect(Buffer* t_buffer, uint64_t t_offset)
	{
		ZoneScoped;

		auto* vkBuffer = static_cast<VulkanBuffer*>(t_buffer);
		vkCmdDispatchIndirect(m_commandBuffer, vkBuffer->GetVkBuffer(), t_offset);
	}

	void VulkanCommandBuffer::ResourceBarrier(const std::vector<BufferBarrier>& t_bufferBarriers, const std::vector<TextureBarrier>& t_textureBarriers)
	{
		ZoneScoped;

		PipelineBarrier(0, 0, t_bufferBarriers, t_textureBarriers);
	}

	void VulkanCommandBuffer::PipelineBarrier(uint32_t t_srcStage, uint32_t t_dstStage, const std::vector<BufferBarrier>& t_bufferBarriers, const std::vector<TextureBarrier>& t_textureBarriers)
	{
		ZoneScoped;

		std::vector<VkBufferMemoryBarrier2> bufferBarriers;
		std::vector<VkImageMemoryBarrier2> imageBarriers;

		for (const auto& barrier : t_bufferBarriers)
		{
			VkBufferMemoryBarrier2 vkBarrier{};
			vkBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
			vkBarrier.srcStageMask = m_device->GetVkPipelineStageFlags2(barrier.srcState);
			vkBarrier.srcAccessMask = m_device->GetVkAccessFlags2(barrier.srcState);
			vkBarrier.dstStageMask = m_device->GetVkPipelineStageFlags2(barrier.dstState);
			vkBarrier.dstAccessMask = m_device->GetVkAccessFlags2(barrier.dstState);
			vkBarrier.buffer = static_cast<VulkanBuffer*>(barrier.buffer)->GetVkBuffer();
			vkBarrier.offset = barrier.offset;
			vkBarrier.size = (barrier.size == 0) ? VK_WHOLE_SIZE : barrier.size;
			bufferBarriers.push_back(vkBarrier);
		}

		for (const auto& barrier : t_textureBarriers)
		{
			auto* vkTexture = static_cast<VulkanTexture*>(barrier.texture);
			VkImageMemoryBarrier2 vkBarrier{};
			vkBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
			vkBarrier.srcStageMask = m_device->GetVkPipelineStageFlags2(barrier.srcState);
			vkBarrier.srcAccessMask = m_device->GetVkAccessFlags2(barrier.srcState);
			vkBarrier.dstStageMask = m_device->GetVkPipelineStageFlags2(barrier.dstState);
			vkBarrier.dstAccessMask = m_device->GetVkAccessFlags2(barrier.dstState);
			vkBarrier.oldLayout = m_device->GetVkImageLayout(barrier.srcState);
			vkBarrier.newLayout = m_device->GetVkImageLayout(barrier.dstState);
			vkBarrier.image = vkTexture->GetVkImage();

			VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			switch (vkTexture->GetAspect())
			{
			case TextureAspect::Color:
				aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				break;
			case TextureAspect::Depth:
				aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				break;
			case TextureAspect::Stencil:
				aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
				break;
			case TextureAspect::DepthStencil:
				aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
				break;
			}

			vkBarrier.subresourceRange.aspectMask = aspectMask;
			vkBarrier.subresourceRange.baseMipLevel = barrier.baseMipLevel;
			vkBarrier.subresourceRange.levelCount = (barrier.levelCount == 0) ? VK_REMAINING_MIP_LEVELS : barrier.levelCount;
			vkBarrier.subresourceRange.baseArrayLayer = barrier.baseArrayLayer;
			vkBarrier.subresourceRange.layerCount = (barrier.layerCount == 0) ? VK_REMAINING_ARRAY_LAYERS : barrier.layerCount;
			imageBarriers.push_back(vkBarrier);
		}

		VkDependencyInfo dependencyInfo{};
		dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
		dependencyInfo.bufferMemoryBarrierCount = static_cast<uint32_t>(bufferBarriers.size());
		dependencyInfo.pBufferMemoryBarriers = bufferBarriers.data();
		dependencyInfo.imageMemoryBarrierCount = static_cast<uint32_t>(imageBarriers.size());
		dependencyInfo.pImageMemoryBarriers = imageBarriers.data();

		vkCmdPipelineBarrier2(m_commandBuffer, &dependencyInfo);
	}

	void VulkanCommandBuffer::CopyBuffer(Buffer* t_srcBuffer, Buffer* t_dstBuffer, uint64_t t_size, uint64_t t_srcOffset, uint64_t t_dstOffset)
	{
		ZoneScoped;

		auto* vkSrcBuffer = static_cast<VulkanBuffer*>(t_srcBuffer);
		auto* vkDstBuffer = static_cast<VulkanBuffer*>(t_dstBuffer);

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = t_srcOffset;
		copyRegion.dstOffset = t_dstOffset;
		copyRegion.size = t_size;

		vkCmdCopyBuffer(m_commandBuffer, vkSrcBuffer->GetVkBuffer(), vkDstBuffer->GetVkBuffer(), 1, &copyRegion);
	}

	static VkImageAspectFlags GetAspectMask(TextureAspect aspect)
	{
		switch (aspect)
		{
		case TextureAspect::Depth:
			return VK_IMAGE_ASPECT_DEPTH_BIT;
		case TextureAspect::Stencil:
			return VK_IMAGE_ASPECT_STENCIL_BIT;
		case TextureAspect::DepthStencil:
			return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		default:
			return VK_IMAGE_ASPECT_COLOR_BIT;
		}
	}

	void VulkanCommandBuffer::CopyBufferToTexture(Buffer* t_srcBuffer, Texture* t_dstTexture, uint32_t t_width, uint32_t t_height, uint32_t t_depth, uint32_t t_bufferRowLength, uint32_t t_bufferImageHeight, uint64_t t_srcOffset, uint32_t t_mipLevel, uint32_t t_arrayLayer)
	{
		ZoneScoped;

		auto* vkSrcBuffer = static_cast<VulkanBuffer*>(t_srcBuffer);
		auto* vkDstTexture = static_cast<VulkanTexture*>(t_dstTexture);

		VkBufferImageCopy region{};
		region.bufferOffset = t_srcOffset;
		region.bufferRowLength = t_bufferRowLength;
		region.bufferImageHeight = t_bufferImageHeight;
		region.imageSubresource.aspectMask = GetAspectMask(t_dstTexture->GetAspect());
		region.imageSubresource.mipLevel = t_mipLevel;
		region.imageSubresource.baseArrayLayer = t_arrayLayer;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { t_width, t_height, t_depth };

		vkCmdCopyBufferToImage(m_commandBuffer, vkSrcBuffer->GetVkBuffer(), vkDstTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	}

	void VulkanCommandBuffer::CopyTextureToBuffer(Texture* t_srcTexture, Buffer* t_dstBuffer, uint32_t t_width, uint32_t t_height, uint32_t t_depth, uint32_t t_bufferRowLength, uint32_t t_bufferImageHeight, uint64_t t_dstOffset, uint32_t t_mipLevel, uint32_t t_arrayLayer)
	{
		ZoneScoped;

		auto* vkSrcTexture = static_cast<VulkanTexture*>(t_srcTexture);
		auto* vkDstBuffer = static_cast<VulkanBuffer*>(t_dstBuffer);

		VkBufferImageCopy region{};
		region.bufferOffset = t_dstOffset;
		region.bufferRowLength = t_bufferRowLength;
		region.bufferImageHeight = t_bufferImageHeight;
		region.imageSubresource.aspectMask = GetAspectMask(t_srcTexture->GetAspect());
		region.imageSubresource.mipLevel = t_mipLevel;
		region.imageSubresource.baseArrayLayer = t_arrayLayer;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { t_width, t_height, t_depth };

		vkCmdCopyImageToBuffer(m_commandBuffer, vkSrcTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkDstBuffer->GetVkBuffer(), 1, &region);
	}

	void VulkanCommandBuffer::CopyTexture(Texture* t_srcTexture, Texture* t_dstTexture, uint32_t t_width, uint32_t t_height, uint32_t t_depth, uint32_t t_srcMipLevel, uint32_t t_dstMipLevel, uint32_t t_srcArrayLayer, uint32_t t_dstArrayLayer)
	{
		ZoneScoped;

		auto* vkSrcTexture = static_cast<VulkanTexture*>(t_srcTexture);
		auto* vkDstTexture = static_cast<VulkanTexture*>(t_dstTexture);

		VkImageCopy region{};
		region.srcSubresource.aspectMask = GetAspectMask(t_srcTexture->GetAspect());
		region.srcSubresource.mipLevel = t_srcMipLevel;
		region.srcSubresource.baseArrayLayer = t_srcArrayLayer;
		region.srcSubresource.layerCount = 1;
		region.dstSubresource.aspectMask = GetAspectMask(t_dstTexture->GetAspect());
		region.dstSubresource.mipLevel = t_dstMipLevel;
		region.dstSubresource.baseArrayLayer = t_dstArrayLayer;
		region.dstSubresource.layerCount = 1;
		region.extent = { t_width, t_height, t_depth };

		vkCmdCopyImage(m_commandBuffer, vkSrcTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkDstTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	}

	void VulkanCommandBuffer::PushConstants(ShaderStage t_shaderStage, uint32_t t_offset, uint32_t t_size, const void* t_data)
	{
		ZoneScoped;

		vkCmdPushConstants(m_commandBuffer, m_pipelineLayout, m_device->GetVkShaderStageFlags(t_shaderStage), t_offset, t_size, t_data);
	}
}