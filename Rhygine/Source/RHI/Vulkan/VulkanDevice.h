#pragma once
#include "IDevice.h"

#include <string>
#include <vector>
#include <memory>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

namespace Rhygine
{
	// Forward declarations
	class VulkanCommandBuffer;
	class VulkanBuffer;
	class VulkanTexture;
	class VulkanShader;
	class VulkanGraphicsPipeline;
	class VulkanComputePipeline;
	class VulkanResourceLayout;
	class VulkanResourceSet;
	class VulkanSwapchain;

	class DescriptorPoolAllocator
	{
	public:
		DescriptorPoolAllocator(VkDevice t_device);
		~DescriptorPoolAllocator();
		VkDescriptorSet Allocate(VkDescriptorSetLayout t_layout);
		void Free(VkDescriptorSet t_set);
	private:
		VkDevice m_device = VK_NULL_HANDLE;
		std::vector<VkDescriptorPool> m_pools;
		VkDescriptorPool m_currentPool = VK_NULL_HANDLE;
		void CreateNewPool();
	};

	/**
	 * VulkanDevice - Vulkan implementation of IDevice
	 */
	class VulkanDevice : public IDevice
	{
	public:
		VulkanDevice();
		~VulkanDevice();

		// =============================================================================
		// IDevice Implementation
		// =============================================================================

		[[nodiscard]] const std::string& GetDeviceName() const override { return m_deviceName; }
		[[nodiscard]] bool IsValid() const override { return m_valid; }

		// Resource Creation
		[[nodiscard]] Buffer* CreateBuffer(const BufferDesc& t_desc) override;
		void DestroyBuffer(Buffer* t_buffer) override;

		[[nodiscard]] Texture* CreateTexture(const TextureDesc& t_desc) override;
		void DestroyTexture(Texture* t_texture) override;

		// Shader
		[[nodiscard]] Shader* CreateShader(const ShaderDesc& t_desc) override;
		void DestroyShader(Shader* t_shader) override;

		// Pipelines
		[[nodiscard]] Pipeline* CreateGraphicsPipeline(const GraphicsPipelineDesc& t_desc) override;
		[[nodiscard]] Pipeline* CreateComputePipeline(const ComputePipelineDesc& t_desc) override;
		void DestroyPipeline(Pipeline* t_pipeline) override;

		// Resource Binding
		[[nodiscard]] ResourceLayout* CreateResourceLayout(const ResourceLayoutDesc& t_desc) override;
		void DestroyResourceLayout(ResourceLayout* t_layout) override;

		[[nodiscard]] ResourceSet* CreateResourceSet(const ResourceSetDesc& t_desc) override;
		void DestroyResourceSet(ResourceSet* t_set) override;

		// Swapchain
		[[nodiscard]] ISwapchain* CreateSwapchain(const SwapchainDesc& t_desc) override;
		void DestroySwapchain(ISwapchain* t_swapchain) override;

		// Command Buffer
		[[nodiscard]] ICommandBuffer* GetCommandBuffer(QueueType t_type = QueueType::Graphics) override;
		[[nodiscard]] ICommandBuffer* GetCommandBuffer(uint32_t t_queueFamilyIndex) override;
		void DestroyCommandBuffer(ICommandBuffer* t_buffer) override;

		// Execution
		void Submit(const std::vector<ICommandBuffer*>& t_buffers, QueueType t_queueType = QueueType::Graphics) override;
		void Submit(const std::vector<ICommandBuffer*>& t_buffers, SyncSemaphore t_waitSemaphore, SyncSemaphore t_signalSemaphore, SyncFence t_signalFence = {}, QueueType t_queueType = QueueType::Graphics) override;
		void WaitIdle() override;

		// Queue Info
		[[nodiscard]] uint32_t GetGraphicsQueueFamilyIndex() const override { return m_graphicsQueueFamilyIndex; }
		[[nodiscard]] uint32_t GetComputeQueueFamilyIndex() const override { return m_computeQueueFamilyIndex; }
		[[nodiscard]] uint32_t GetTransferQueueFamilyIndex() const override { return m_transferQueueFamilyIndex; }

		// Memory
		[[nodiscard]] uint64_t GetTotalMemory() const override;
		[[nodiscard]] uint64_t GetAvailableMemory() const override;

		// =============================================================================
		// Vulkan-specific methods
		// =============================================================================

		[[nodiscard]] VkInstance GetVkInstance() const { return m_instance; }
		[[nodiscard]] VkDevice GetVkDevice() const { return m_device; }
		[[nodiscard]] VkPhysicalDevice GetVkPhysicalDevice() const { return m_physicalDevice; }
		[[nodiscard]] VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }
		[[nodiscard]] VkQueue GetComputeQueue() const { return m_computeQueue; }
		[[nodiscard]] VkQueue GetTransferQueue() const { return m_transferQueue; }
		[[nodiscard]] VmaAllocator GetVmaAllocator() const { return m_vmaAllocator; }
		[[nodiscard]] VkSampler GetVkDefaultSampler() const { return m_defaultSampler; }
		[[nodiscard]] DescriptorPoolAllocator* GetDescriptorPoolAllocator() const { return m_descriptorPoolAllocator.get(); }

		// Format mapping
		[[nodiscard]] VkFormat GetVkFormat(Format t_format) const;
		[[nodiscard]] Format GetFormat(VkFormat t_format) const;

		// State mapping
		[[nodiscard]] VkImageLayout GetVkImageLayout(ResourceState t_state) const;
		[[nodiscard]] VkAccessFlags GetVkAccessFlags(ResourceState t_state) const;
		[[nodiscard]] VkPipelineStageFlags GetVkPipelineStageFlags(ResourceState t_state) const;
		[[nodiscard]] VkPipelineStageFlags2 GetVkPipelineStageFlags2(ResourceState t_state) const;
		[[nodiscard]] VkAccessFlags2 GetVkAccessFlags2(ResourceState t_state) const;

		// Usage mapping
		[[nodiscard]] VkBufferUsageFlags GetVkBufferUsageFlags(BufferUsage t_usage) const;
		[[nodiscard]] VkImageUsageFlags GetVkImageUsageFlags(TextureUsage t_usage) const;

		// Shader stage mapping
		[[nodiscard]] VkShaderStageFlags GetVkShaderStageFlags(ShaderStage t_stage) const;

		// Debug utils naming
		void SetDebugName(VkObjectType t_objectType, uint64_t t_objectHandle, const std::string& t_name) const;

	private:
		// Initialization
		void Initialize();
		void CreateInstance();
		void CreatePhysicalDevice();
		void CreateDevice();
		void CreateQueues();
		void InitializeVMA();
		void CreateDefaultSampler();
		void Cleanup();

		// Helper methods
		bool CheckValidationLayerSupport() const;
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT t_messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT t_messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* t_callbackData,
			void* t_userData);
		void SetupDebugMessenger();

		std::vector<const char*> GetRequiredExtensions() const;
		uint32_t ScorePhysicalDevice(VkPhysicalDevice t_device) const;
		QueueType GetQueueTypeFromFamilyIndex(uint32_t t_familyIndex) const;

		// Member variables
		VkInstance m_instance = VK_NULL_HANDLE;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkDevice m_device = VK_NULL_HANDLE;
		VmaAllocator m_vmaAllocator = VK_NULL_HANDLE;

		VkQueue m_graphicsQueue = VK_NULL_HANDLE;
		VkQueue m_computeQueue = VK_NULL_HANDLE;
		VkQueue m_transferQueue = VK_NULL_HANDLE;

		uint32_t m_graphicsQueueFamilyIndex = 0;
		uint32_t m_computeQueueFamilyIndex = 0;
		uint32_t m_transferQueueFamilyIndex = 0;

		VkSampler m_defaultSampler = VK_NULL_HANDLE;

		std::unique_ptr<DescriptorPoolAllocator> m_descriptorPoolAllocator;

		VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
		PFN_vkSetDebugUtilsObjectNameEXT m_pfnSetDebugUtilsObjectNameEXT = nullptr;

		std::string m_deviceName;
		bool m_valid = false;
		bool m_enableValidationLayers = false;
	};
}