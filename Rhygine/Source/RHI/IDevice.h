#pragma once

#include "RHIDescriptors.h"
#include "ICommandBuffer.h"
#include "ISwapchain.h"

#include <vector>
#include <memory>
#include <string>

namespace Rhygine
{
	// Forward declarations
	class Buffer;
	class Texture;
	class Shader;
	class Pipeline;
	class ResourceLayout;
	class ResourceSet;
	class ISwapchain;

	/**
	 * IDevice - The central factory and manager for all GPU resources.
	 * It does not do any drawing itself, but provides the interface for creating
	 * and managing all GPU resources.
	 * 
	 * This is the primary interface for interacting with the graphics API.
	 */
	class IDevice
	{
	public:
		virtual ~IDevice() = default;

		// =============================================================================
		// Device Info
		// =============================================================================

		/// <summary>
		/// Gets the name of the GPU device.
		/// </summary>
		/// <returns>String containing the device name.</returns>
		[[nodiscard]] virtual const std::string& GetDeviceName() const = 0;

		/// <summary>
		/// Checks if the device is valid and ready for use.
		/// </summary>
		/// <returns>True if device is ready.</returns>
		[[nodiscard]] virtual bool IsValid() const = 0;

		// =============================================================================
		// Resource Creation - Buffers
		// =============================================================================

		/// <summary>
		/// Creates a new buffer.
		/// </summary>
		/// <param name="t_desc">Buffer descriptor containing creation parameters.</param>
		/// <returns>Pointer to the created buffer, or nullptr on failure.</returns>
		[[nodiscard]] virtual Buffer* CreateBuffer(const BufferDesc& t_desc) = 0;

		/// <summary>
		/// Destroys a buffer.
		/// </summary>
		/// <param name="t_buffer">Pointer to the buffer to destroy.</param>
		virtual void DestroyBuffer(Buffer* t_buffer) = 0;

		// =============================================================================
		// Resource Creation - Textures
		// =============================================================================

		/// <summary>
		/// Creates a new texture.
		/// </summary>
		/// <param name="t_desc">Texture descriptor containing creation parameters.</param>
		/// <returns>Pointer to the created texture, or nullptr on failure.</returns>
		[[nodiscard]] virtual Texture* CreateTexture(const TextureDesc& t_desc) = 0;

		/// <summary>
		/// Destroys a texture.
		/// </summary>
		/// <param name="t_texture">Pointer to the texture to destroy.</param>
		virtual void DestroyTexture(Texture* t_texture) = 0;

		// =============================================================================
		// Pipeline Creation - Shaders
		// =============================================================================

		/// <summary>
		/// Creates a new shader from compiled bytecode.
		/// </summary>
		/// <param name="t_desc">Shader descriptor containing bytecode and entry point.</param>
		/// <returns>Pointer to the created shader, or nullptr on failure.</returns>
		[[nodiscard]] virtual Shader* CreateShader(const ShaderDesc& t_desc) = 0;

		/// <summary>
		/// Destroys a shader.
		/// </summary>
		/// <param name="t_shader">Pointer to the shader to destroy.</param>
		virtual void DestroyShader(Shader* t_shader) = 0;

		// =============================================================================
		// Pipeline Creation - Graphics
		// =============================================================================

		/// <summary>
		/// Creates a new graphics pipeline.
		/// </summary>
		/// <param name="t_desc">Graphics pipeline descriptor containing all state.</param>
		/// <returns>Pointer to the created pipeline, or nullptr on failure.</returns>
		[[nodiscard]] virtual Pipeline* CreateGraphicsPipeline(const GraphicsPipelineDesc& t_desc) = 0;

		// =============================================================================
		// Pipeline Creation - Compute
		// =============================================================================

		/// <summary>
		/// Creates a new compute pipeline.
		/// </summary>
		/// <param name="t_desc">Compute pipeline descriptor containing compute shader.</param>
		/// <returns>Pointer to the created pipeline, or nullptr on failure.</returns>
		[[nodiscard]] virtual Pipeline* CreateComputePipeline(const ComputePipelineDesc& t_desc) = 0;

		/// <summary>
		/// Destroys a pipeline.
		/// </summary>
		/// <param name="t_pipeline">Pointer to the pipeline to destroy.</param>
		virtual void DestroyPipeline(Pipeline* t_pipeline) = 0;

		// =============================================================================
		// Resource Binding - Layout
		// =============================================================================

		/// <summary>
		/// Creates a new resource layout defining what will be bound.
		/// </summary>
		/// <param name="t_desc">Resource layout descriptor containing bindings.</param>
		/// <returns>Pointer to the created resource layout, or nullptr on failure.</returns>
		[[nodiscard]] virtual ResourceLayout* CreateResourceLayout(const ResourceLayoutDesc& t_desc) = 0;

		/// <summary>
		/// Destroys a resource layout.
		/// </summary>
		/// <param name="t_layout">Pointer to the resource layout to destroy.</param>
		virtual void DestroyResourceLayout(ResourceLayout* t_layout) = 0;

		// =============================================================================
		// Resource Binding - Sets
		// =============================================================================

		/// <summary>
		/// Creates a new resource set containing actual resource pointers.
		/// </summary>
		/// <param name="t_desc">Resource set descriptor containing layout and resources.</param>
		/// <returns>Pointer to the created resource set, or nullptr on failure.</returns>
		[[nodiscard]] virtual ResourceSet* CreateResourceSet(const ResourceSetDesc& t_desc) = 0;

		/// <summary>
		/// Destroys a resource set.
		/// </summary>
		/// <param name="t_set">Pointer to the resource set to destroy.</param>
		virtual void DestroyResourceSet(ResourceSet* t_set) = 0;

		// =============================================================================
		// Swapchain Management
		// =============================================================================

		/// <summary>
		/// Creates a new swapchain for a window.
		/// </summary>
		/// <param name="t_desc">Swapchain descriptor containing parameters.</param>
		/// <returns>Pointer to the created swapchain, or nullptr on failure.</returns>
		[[nodiscard]] virtual ISwapchain* CreateSwapchain(const SwapchainDesc& t_desc) = 0;

		/// <summary>
		/// Destroys a swapchain.
		/// </summary>
		/// <param name="t_swapchain">Pointer to the swapchain to destroy.</param>
		virtual void DestroySwapchain(ISwapchain* t_swapchain) = 0;

		// =============================================================================
		// Command Buffer Management
		// =============================================================================

		/// <summary>
		/// Gets a command buffer for recording commands.
		/// </summary>
		/// <param name="t_type">Type of queue to get command buffer for.</param>
		/// <returns>Pointer to the command buffer.</returns>
		[[nodiscard]] virtual ICommandBuffer* GetCommandBuffer(QueueType t_type = QueueType::Graphics) = 0;

		/// <summary>
		/// Gets a command buffer for a specific queue family index.
		/// </summary>
		/// <param name="t_queueFamilyIndex">Queue family index.</param>
		/// <returns>Pointer to the command buffer.</returns>
		[[nodiscard]] virtual ICommandBuffer* GetCommandBuffer(uint32_t t_queueFamilyIndex) = 0;

		/// <summary>
		/// Destroys a command buffer.
		/// </summary>
		/// <param name="t_buffer">Pointer to the command buffer to destroy.</param>
		virtual void DestroyCommandBuffer(ICommandBuffer* t_buffer) = 0;

		// =============================================================================
		// Execution & Synchronization
		// =============================================================================

		/// <summary>
		/// Submits command buffers to the GPU for execution.
		/// </summary>
		/// <param name="t_buffers">Vector of command buffers to submit.</param>
		/// <param name="t_queueType">Queue type to submit to.</param>
		virtual void Submit(const std::vector<ICommandBuffer*>& t_buffers, QueueType t_queueType = QueueType::Graphics) = 0;

		/// <summary>
		/// Submits command buffers with wait semaphore, signal semaphore, and optional signal fence.
		/// </summary>
		/// <param name="t_buffers">Vector of command buffers to submit.</param>
		/// <param name="t_waitSemaphore">Semaphore to wait on before executing.</param>
		/// <param name="t_signalSemaphore">Semaphore to signal when execution is complete.</param>
		/// <param name="t_signalFence">Fence to signal when execution is complete.</param>
		/// <param name="t_queueType">Queue type to submit to.</param>
		virtual void Submit(const std::vector<ICommandBuffer*>& t_buffers, SyncSemaphore t_waitSemaphore, SyncSemaphore t_signalSemaphore, SyncFence t_signalFence = {}, QueueType t_queueType = QueueType::Graphics) = 0;

		/// <summary>
		/// Waits for all GPU operations to complete.
		/// </summary>
		virtual void WaitIdle() = 0;

		/// <summary>
		/// Gets the graphics queue family index.
		/// </summary>
		/// <returns>Graphics queue family index.</returns>
		[[nodiscard]] virtual uint32_t GetGraphicsQueueFamilyIndex() const = 0;

		/// <summary>
		/// Gets the compute queue family index.
		/// </summary>
		/// <returns>Compute queue family index.</returns>
		[[nodiscard]] virtual uint32_t GetComputeQueueFamilyIndex() const = 0;

		/// <summary>
		/// Gets the transfer queue family index.
		/// </summary>
		/// <returns>Transfer queue family index.</returns>
		[[nodiscard]] virtual uint32_t GetTransferQueueFamilyIndex() const = 0;

		/// <summary>
		/// Creates a GPU device instance for the specified backend.
		/// </summary>
		/// <param name="t_backend">Graphics backend to instantiate.</param>
		/// <returns>Unique pointer to created device.</returns>
		static std::unique_ptr<IDevice> Create(DeviceBackend t_backend = DeviceBackend::Vulkan);

		// =============================================================================
		// Memory Management
		// =============================================================================

		/// <summary>
		/// Gets the total available GPU memory.
		/// </summary>
		/// <returns>Total memory in bytes.</returns>
		[[nodiscard]] virtual uint64_t GetTotalMemory() const = 0;

		/// <summary>
		/// Gets the available free GPU memory.
		/// </summary>
		/// <returns>Free memory in bytes.</returns>
		[[nodiscard]] virtual uint64_t GetAvailableMemory() const = 0;
	};

	// =============================================================================
	// Base Classes for GPU Resources
	// =============================================================================

	/// <summary>
	/// Buffer - GPU buffer resource (vertex, index, constant, storage).
	/// </summary>
	class Buffer
	{
	public:
		virtual ~Buffer() = default;

		[[nodiscard]] virtual uint64_t GetSize() const = 0;
		[[nodiscard]] virtual BufferUsage GetUsage() const = 0;
		[[nodiscard]] virtual ResourceState GetState() const = 0;
		virtual void SetState(ResourceState t_state) = 0;

		/// <summary>
		/// Maps host-visible buffer memory to CPU virtual address space.
		/// </summary>
		/// <returns>Pointer to mapped CPU memory, or nullptr on failure.</returns>
		virtual void* Map() { return nullptr; }

		/// <summary>
		/// Unmaps host-visible buffer memory.
		/// </summary>
		virtual void Unmap() {}

	protected:
		BufferDesc m_desc;
		ResourceState m_currentState = ResourceState::Undefined;
	};

	/// <summary>
	/// Texture - GPU texture resource (1D, 2D, 3D, cube).
	/// </summary>
	class Texture
	{
	public:
		virtual ~Texture() = default;

		[[nodiscard]] virtual uint32_t GetWidth() const = 0;
		[[nodiscard]] virtual uint32_t GetHeight() const = 0;
		[[nodiscard]] virtual uint32_t GetDepth() const = 0;
		[[nodiscard]] virtual uint32_t GetMipLevels() const = 0;
		[[nodiscard]] virtual Format GetFormat() const = 0;
		[[nodiscard]] virtual TextureType GetType() const = 0;
		[[nodiscard]] virtual TextureUsage GetUsage() const = 0;
		[[nodiscard]] virtual TextureAspect GetAspect() const = 0;
		[[nodiscard]] virtual ResourceState GetState() const = 0;
		virtual void SetState(ResourceState t_state) = 0;

	protected:
		TextureDesc m_desc;
		ResourceState m_currentState = ResourceState::Undefined;
	};

	/// <summary>
	/// Shader - Compiled shader bytecode.
	/// </summary>
	class Shader
	{
	public:
		virtual ~Shader() = default;

		[[nodiscard]] virtual ShaderStage GetStage() const = 0;
		[[nodiscard]] virtual const std::string& GetEntryPoint() const = 0;

	protected:
		ShaderDesc m_desc;
	};

	/// <summary>
	/// Pipeline - Graphics or compute pipeline state object.
	/// </summary>
	class Pipeline
	{
	public:
		virtual ~Pipeline() = default;

		[[nodiscard]] virtual bool IsGraphics() const = 0;
		[[nodiscard]] virtual bool IsCompute() const = 0;

	protected:
		bool m_isGraphics = true;
	};

	/// <summary>
	/// ResourceLayout - Defines what resources will be bound (maps to VkDescriptorSetLayout).
	/// </summary>
	class ResourceLayout
	{
	public:
		virtual ~ResourceLayout() = default;

		[[nodiscard]] virtual const std::vector<ResourceLayoutBinding>& GetBindings() const = 0;

	protected:
		ResourceLayoutDesc m_desc;
	};

	/// <summary>
	/// ResourceSet - Actual instance of a layout containing pointers to resources (maps to VkDescriptorSet).
	/// </summary>
	class ResourceSet
	{
	public:
		virtual ~ResourceSet() = default;

		[[nodiscard]] virtual ResourceLayout* GetLayout() const = 0;

		/// <summary>
		/// Updates a buffer binding in this resource set.
		/// </summary>
		virtual void UpdateBuffer(uint32_t t_binding, Buffer* t_buffer, uint64_t t_offset = 0, uint64_t t_range = 0) = 0;

		/// <summary>
		/// Updates a texture binding in this resource set.
		/// </summary>
		virtual void UpdateTexture(uint32_t t_binding, Texture* t_texture) = 0;

	protected:
		ResourceSetDesc m_desc;
	};
}