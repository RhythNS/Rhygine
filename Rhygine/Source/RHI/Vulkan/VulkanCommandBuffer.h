#pragma once
#include "ICommandBuffer.h"

#include <vulkan/vulkan.h>

namespace Rhygine
{
	class VulkanDevice;

	/**
	 * VulkanCommandBuffer - Vulkan implementation of ICommandBuffer
	 */
	class VulkanCommandBuffer : public ICommandBuffer
	{
	public:
		VulkanCommandBuffer(VulkanDevice* t_device, uint32_t t_queueFamilyIndex);
		~VulkanCommandBuffer();

		// ICommandBuffer interface
		void Begin() override;
		void End() override;

		void BindPipeline(Pipeline* t_pipeline) override;
		void BindResourceSet(uint32_t t_slot, ResourceSet* t_set) override;
		void BindVertexBuffer(uint32_t t_slot, Buffer* t_buffer, uint64_t t_offset = 0) override;
		void BindIndexBuffer(Buffer* t_buffer, uint64_t t_offset = 0, IndexFormat t_format = IndexFormat::Uint32) override;

		void SetViewport(const Viewport& t_viewport) override;
		void SetScissor(const ScissorRect& t_scissor) override;

		void BeginRenderPass(const RenderPassBeginInfo& t_info) override;
		void EndRenderPass() override;

		void Draw(uint32_t t_vertexCount, uint32_t t_instanceCount = 1, uint32_t t_firstVertex = 0, uint32_t t_firstInstance = 0) override;
		void DrawIndexed(uint32_t t_indexCount, uint32_t t_instanceCount = 1, uint32_t t_firstIndex = 0, int32_t t_vertexOffset = 0, uint32_t t_firstInstance = 0) override;
		void DrawIndirect(Buffer* t_buffer, uint64_t t_offset = 0, uint32_t t_drawCount = 1, uint32_t t_stride = 0) override;
		void DrawIndexedIndirect(Buffer* t_buffer, uint64_t t_offset = 0, uint32_t t_drawCount = 1, uint32_t t_stride = 0) override;

		void Dispatch(uint32_t t_groupCountX, uint32_t t_groupCountY, uint32_t t_groupCountZ = 1) override;
		void DispatchIndirect(Buffer* t_buffer, uint64_t t_offset = 0) override;

		void ResourceBarrier(const std::vector<BufferBarrier>& t_bufferBarriers, const std::vector<TextureBarrier>& t_textureBarriers) override;
		void PipelineBarrier(uint32_t t_srcStage, uint32_t t_dstStage, const std::vector<BufferBarrier>& t_bufferBarriers, const std::vector<TextureBarrier>& t_textureBarriers) override;

		void CopyBuffer(Buffer* t_srcBuffer, Buffer* t_dstBuffer, uint64_t t_size, uint64_t t_srcOffset = 0, uint64_t t_dstOffset = 0) override;
		void CopyBufferToTexture(Buffer* t_srcBuffer, Texture* t_dstTexture, uint32_t t_width, uint32_t t_height, uint32_t t_depth = 1, uint32_t t_bufferRowLength = 0, uint32_t t_bufferImageHeight = 0, uint64_t t_srcOffset = 0, uint32_t t_mipLevel = 0, uint32_t t_arrayLayer = 0) override;
		void CopyTextureToBuffer(Texture* t_srcTexture, Buffer* t_dstBuffer, uint32_t t_width, uint32_t t_height, uint32_t t_depth = 1, uint32_t t_bufferRowLength = 0, uint32_t t_bufferImageHeight = 0, uint64_t t_dstOffset = 0, uint32_t t_mipLevel = 0, uint32_t t_arrayLayer = 0) override;
		void CopyTexture(Texture* t_srcTexture, Texture* t_dstTexture, uint32_t t_width, uint32_t t_height, uint32_t t_depth = 1, uint32_t t_srcMipLevel = 0, uint32_t t_dstMipLevel = 0, uint32_t t_srcArrayLayer = 0, uint32_t t_dstArrayLayer = 0) override;

		void PushConstants(ShaderStage t_shaderStage, uint32_t t_offset, uint32_t t_size, const void* t_data) override;

		// Vulkan-specific
		[[nodiscard]] VkCommandBuffer GetVkCommandBuffer() const { return m_commandBuffer; }

	private:
		VulkanDevice* m_device;
		VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
		VkCommandPool m_commandPool = VK_NULL_HANDLE;
		VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
		bool m_isComputePipeline = false;
	};
}