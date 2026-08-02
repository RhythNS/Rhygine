#pragma once

#include "RHIDescriptors.h"

namespace Rhygine
{
	// Forward declarations
	class Pipeline;
	class ResourceSet;
	class Buffer;
	class Texture;

	/**
	 * ICommandBuffer - The workhorse interface used to record instructions for the GPU.
	 * All rendering and compute operations are recorded into command buffers,
	 * then submitted to a queue.
	 */
	class ICommandBuffer
	{
	public:
		virtual ~ICommandBuffer() = default;

		// =============================================================================
		// Command Buffer Lifecycle
		// =============================================================================
		
		/// <summary>
		/// Begins recording commands into this command buffer.
		/// Must be called before any other recording commands.
		/// </summary>
		virtual void Begin() = 0;

		/// <summary>
		/// Ends recording commands into this command buffer.
		/// Must be called after all recording commands are complete.
		/// </summary>
		virtual void End() = 0;

		// =============================================================================
		// Pipeline & State Binding
		// =============================================================================

		/// <summary>
		/// Binds a graphics or compute pipeline to the command buffer.
		/// </summary>
		/// <param name="t_pipeline">The pipeline to bind.</param>
		virtual void BindPipeline(Pipeline* t_pipeline) = 0;

		/// <summary>
		/// Binds a resource set to a specific slot.
		/// </summary>
		/// <param name="t_slot">The slot/binding point for the resource set.</param>
		/// <param name="t_set">The resource set containing the actual resource bindings.</param>
		virtual void BindResourceSet(uint32_t t_slot, ResourceSet* t_set) = 0;

		// =============================================================================
		// Vertex & Index Buffer Binding
		// =============================================================================

		/// <summary>
		/// Binds a vertex buffer to a specific slot.
		/// </summary>
		/// <param name="t_slot">The vertex input slot index.</param>
		/// <param name="t_buffer">The vertex buffer to bind.</param>
		/// <param name="t_offset">Byte offset from the start of the buffer.</param>
		virtual void BindVertexBuffer(uint32_t t_slot, Buffer* t_buffer, uint64_t t_offset = 0) = 0;

		/// <summary>
		/// Binds an index buffer.
		/// </summary>
		/// <param name="t_buffer">The index buffer to bind.</param>
		/// <param name="t_offset">Byte offset from the start of the buffer.</param>
		/// <param name="t_format">The format of the index data (Uint16 or Uint32).</param>
		virtual void BindIndexBuffer(Buffer* t_buffer, uint64_t t_offset = 0, IndexFormat t_format = IndexFormat::Uint32) = 0;

		// =============================================================================
		// Viewport & Scissor
		// =============================================================================

		/// <summary>
		/// Sets the viewport for subsequent rendering operations.
		/// </summary>
		/// <param name="t_viewport">The viewport configuration.</param>
		virtual void SetViewport(const Viewport& t_viewport) = 0;

		/// <summary>
		/// Sets the scissor rectangle for subsequent rendering operations.
		/// </summary>
		/// <param name="t_scissor">The scissor rectangle.</param>
		virtual void SetScissor(const ScissorRect& t_scissor) = 0;

		// =============================================================================
		// Render Pass (Dynamic Rendering)
		// =============================================================================

		/// <summary>
		/// Begins a dynamic render pass.
		/// Uses VK_KHR_dynamic_rendering / DirectX 12 dynamic rendering approach.
		/// </summary>
		/// <param name="t_info">Render pass begin info containing color and depth targets.</param>
		virtual void BeginRenderPass(const RenderPassBeginInfo& t_info) = 0;

		/// <summary>
		/// Ends the current render pass.
		/// </summary>
		virtual void EndRenderPass() = 0;

		// =============================================================================
		// Draw Commands
		// =============================================================================

		/// <summary>
		/// Draws non-indexed primitives.
		/// </summary>
		/// <param name="t_vertexCount">Number of vertices to draw.</param>
		/// <param name="t_instanceCount">Number of instances to draw.</param>
		/// <param name="t_firstVertex">Index of the first vertex to draw.</param>
		/// <param name="t_firstInstance">Instance ID of the first instance to draw.</param>
		virtual void Draw(uint32_t t_vertexCount, uint32_t t_instanceCount = 1, uint32_t t_firstVertex = 0, uint32_t t_firstInstance = 0) = 0;

		/// <summary>
		/// Draws indexed primitives.
		/// </summary>
		/// <param name="t_indexCount">Number of indices to draw.</param>
		/// <param name="t_instanceCount">Number of instances to draw.</param>
		/// <param name="t_firstIndex">Index of the first index to draw.</param>
		/// <param name="t_vertexOffset">Value added to the index buffer indices.</param>
		/// <param name="t_firstInstance">Instance ID of the first instance to draw.</param>
		virtual void DrawIndexed(uint32_t t_indexCount, uint32_t t_instanceCount = 1, uint32_t t_firstIndex = 0, int32_t t_vertexOffset = 0, uint32_t t_firstInstance = 0) = 0;

		/// <summary>
		/// Draws primitives with indirect parameters from a buffer.
		/// </summary>
		/// <param name="t_buffer">Buffer containing draw indirect parameters.</param>
		/// <param name="t_offset">Byte offset in the indirect buffer.</param>
		/// <param name="t_drawCount">Number of draws to execute.</param>
		/// <param name="t_stride">Stride between draw parameters.</param>
		virtual void DrawIndirect(Buffer* t_buffer, uint64_t t_offset = 0, uint32_t t_drawCount = 1, uint32_t t_stride = 0) = 0;

		/// <summary>
		/// Draws indexed primitives with indirect parameters from a buffer.
		/// </summary>
		/// <param name="t_buffer">Buffer containing draw indexed indirect parameters.</param>
		/// <param name="t_offset">Byte offset in the indirect buffer.</param>
		/// <param name="t_drawCount">Number of draws to execute.</param>
		/// <param name="t_stride">Stride between draw parameters.</param>
		virtual void DrawIndexedIndirect(Buffer* t_buffer, uint64_t t_offset = 0, uint32_t t_drawCount = 1, uint32_t t_stride = 0) = 0;

		// =============================================================================
		// Compute Commands
		// =============================================================================

		/// <summary>
		/// Dispatches a compute shader.
		/// </summary>
		/// <param name="t_groupCountX">Number of thread groups in X dimension.</param>
		/// <param name="t_groupCountY">Number of thread groups in Y dimension.</param>
		/// <param name="t_groupCountZ">Number of thread groups in Z dimension.</param>
		virtual void Dispatch(uint32_t t_groupCountX, uint32_t t_groupCountY, uint32_t t_groupCountZ = 1) = 0;

		/// <summary>
		/// Dispatches a compute shader with indirect parameters from a buffer.
		/// </summary>
		/// <param name="t_buffer">Buffer containing dispatch indirect parameters.</param>
		/// <param name="t_offset">Byte offset in the indirect buffer.</param>
		virtual void DispatchIndirect(Buffer* t_buffer, uint64_t t_offset = 0) = 0;

		// =============================================================================
		// Synchronization
		// =============================================================================

		/// <summary>
		/// Inserts resource barriers for buffers and textures.
		/// Handles state transitions for synchronization.
		/// </summary>
		/// <param name="t_bufferBarriers">Vector of buffer barriers.</param>
		/// <param name="t_textureBarriers">Vector of texture barriers.</param>
		virtual void ResourceBarrier(const std::vector<BufferBarrier>& t_bufferBarriers, const std::vector<TextureBarrier>& t_textureBarriers) = 0;

		/// <summary>
		/// Inserts a pipeline barrier with specific stage masks.
		/// </summary>
		/// <param name="t_srcStage">Source pipeline stage.</param>
		/// <param name="t_dstStage">Destination pipeline stage.</param>
		/// <param name="t_bufferBarriers">Vector of buffer barriers.</param>
		/// <param name="t_textureBarriers">Vector of texture barriers.</param>
		virtual void PipelineBarrier(uint32_t t_srcStage, uint32_t t_dstStage, const std::vector<BufferBarrier>& t_bufferBarriers, const std::vector<TextureBarrier>& t_textureBarriers) = 0;

		// =============================================================================
		// Copy Commands
		// =============================================================================

		/// <summary>
		/// Copies data from one buffer to another.
		/// </summary>
		/// <param name="t_srcBuffer">Source buffer.</param>
		/// <param name="t_dstBuffer">Destination buffer.</param>
		/// <param name="t_size">Size of data to copy in bytes.</param>
		/// <param name="t_srcOffset">Offset in source buffer.</param>
		/// <param name="t_dstOffset">Offset in destination buffer.</param>
		virtual void CopyBuffer(Buffer* t_srcBuffer, Buffer* t_dstBuffer, uint64_t t_size, uint64_t t_srcOffset = 0, uint64_t t_dstOffset = 0) = 0;

		/// <summary>
		/// Copies data from a buffer to a texture.
		/// </summary>
		/// <param name="t_srcBuffer">Source buffer.</param>
		/// <param name="t_dstTexture">Destination texture.</param>
		/// <param name="t_width">Width of the region to copy.</param>
		/// <param name="t_height">Height of the region to copy.</param>
		/// <param name="t_depth">Depth of the region to copy.</param>
		/// <param name="t_bufferRowLength">Row length of the buffer data.</param>
		/// <param name="t_bufferImageHeight">Image height of the buffer data.</param>
		/// <param name="t_srcOffset">Offset in source buffer.</param>
		/// <param name="t_mipLevel">Mip level to copy to.</param>
		/// <param name="t_arrayLayer">Array layer to copy to.</param>
		virtual void CopyBufferToTexture(Buffer* t_srcBuffer, Texture* t_dstTexture, uint32_t t_width, uint32_t t_height, uint32_t t_depth = 1, 
			uint32_t t_bufferRowLength = 0, uint32_t t_bufferImageHeight = 0, uint64_t t_srcOffset = 0, uint32_t t_mipLevel = 0, uint32_t t_arrayLayer = 0) = 0;

		/// <summary>
		/// Copies data from a texture to a buffer.
		/// </summary>
		/// <param name="t_srcTexture">Source texture.</param>
		/// <param name="t_dstBuffer">Destination buffer.</param>
		/// <param name="t_width">Width of the region to copy.</param>
		/// <param name="t_height">Height of the region to copy.</param>
		/// <param name="t_depth">Depth of the region to copy.</param>
		/// <param name="t_bufferRowLength">Row length of the buffer data.</param>
		/// <param name="t_bufferImageHeight">Image height of the buffer data.</param>
		/// <param name="t_dstOffset">Offset in destination buffer.</param>
		/// <param name="t_mipLevel">Mip level to copy from.</param>
		/// <param name="t_arrayLayer">Array layer to copy from.</param>
		virtual void CopyTextureToBuffer(Texture* t_srcTexture, Buffer* t_dstBuffer, uint32_t t_width, uint32_t t_height, uint32_t t_depth = 1,
			uint32_t t_bufferRowLength = 0, uint32_t t_bufferImageHeight = 0, uint64_t t_dstOffset = 0, uint32_t t_mipLevel = 0, uint32_t t_arrayLayer = 0) = 0;

		/// <summary>
		/// Copies data from one texture to another.
		/// </summary>
		/// <param name="t_srcTexture">Source texture.</param>
		/// <param name="t_dstTexture">Destination texture.</param>
		/// <param name="t_width">Width of the region to copy.</param>
		/// <param name="t_height">Height of the region to copy.</param>
		/// <param name="t_depth">Depth of the region to copy.</param>
		/// <param name="t_srcMipLevel">Source mip level.</param>
		/// <param name="t_dstMipLevel">Destination mip level.</param>
		/// <param name="t_srcArrayLayer">Source array layer.</param>
		/// <param name="t_dstArrayLayer">Destination array layer.</param>
		virtual void CopyTexture(Texture* t_srcTexture, Texture* t_dstTexture, uint32_t t_width, uint32_t t_height, uint32_t t_depth = 1,
			uint32_t t_srcMipLevel = 0, uint32_t t_dstMipLevel = 0, uint32_t t_srcArrayLayer = 0, uint32_t t_dstArrayLayer = 0) = 0;

		// =============================================================================
		// Push Constants
		// =============================================================================

		/// <summary>
		/// Pushes constant data to the shader.
		/// </summary>
		/// <param name="t_shaderStage">Shader stage to receive the constants.</param>
		/// <param name="t_offset">Byte offset in the push constant range.</param>
		/// <param name="t_size">Size of the data to push.</param>
		/// <param name="t_data">Pointer to the data to push.</param>
		virtual void PushConstants(ShaderStage t_shaderStage, uint32_t t_offset, uint32_t t_size, const void* t_data) = 0;
	};
}