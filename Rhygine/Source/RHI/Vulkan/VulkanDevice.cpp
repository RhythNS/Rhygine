#include "VulkanDevice.h"

#include <iostream>
#include <set>
#include <algorithm>
#include <optional>
#include <unordered_map>
#include <tracy/Tracy.hpp>

#include "Debug/Error.h"
#include "Debug/Logger.h"
#include "VulkanCommandBuffer.h"
#include "VulkanBuffer.h"
#include "VulkanTexture.h"
#include "VulkanShader.h"
#include "VulkanPipeline.h"
#include "VulkanResourceLayout.h"
#include "VulkanResourceSet.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"
#include "VulkanCheck.h"

// Validation layer names
const std::vector<const char*> VALIDATION_LAYERS = {
	"VK_LAYER_KHRONOS_validation"
};

// Required device extensions
const std::vector<const char*> REQUIRED_DEVICE_EXTENSIONS = {
	VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace Rhygine
{
	DescriptorPoolAllocator::DescriptorPoolAllocator(VkDevice t_device)
		: m_device(t_device)
	{
	}

	DescriptorPoolAllocator::~DescriptorPoolAllocator()
	{
		for (auto pool : m_pools)
		{
			if (pool != VK_NULL_HANDLE)
			{
				vkDestroyDescriptorPool(m_device, pool, nullptr);
			}
		}
	}

	void DescriptorPoolAllocator::CreateNewPool()
	{
		std::vector<VkDescriptorPoolSize> poolSizes = {
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 256 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 256 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 128 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 64 },
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 64 },
		};

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = 128;

		VkDescriptorPool pool = VK_NULL_HANDLE;
		VK_CHECK(vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &pool));

		m_pools.push_back(pool);
		m_currentPool = pool;
	}

	VkDescriptorSet DescriptorPoolAllocator::Allocate(VkDescriptorSetLayout t_layout)
	{
		if (m_currentPool == VK_NULL_HANDLE)
		{
			CreateNewPool();
		}

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_currentPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &t_layout;

		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
		VkResult result = vkAllocateDescriptorSets(m_device, &allocInfo, &descriptorSet);

		if (result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL)
		{
			CreateNewPool();
			allocInfo.descriptorPool = m_currentPool;
			VK_CHECK(vkAllocateDescriptorSets(m_device, &allocInfo, &descriptorSet));
		}
		else
		{
			VK_CHECK(result);
		}

		return descriptorSet;
	}

	void DescriptorPoolAllocator::Free(VkDescriptorSet t_set)
	{
		if (t_set != VK_NULL_HANDLE && m_currentPool != VK_NULL_HANDLE)
		{
			vkFreeDescriptorSets(m_device, m_currentPool, 1, &t_set);
		}
	}

	std::unique_ptr<IDevice> IDevice::Create(DeviceBackend t_backend)
	{
		if (t_backend == DeviceBackend::Vulkan)
		{
			return std::make_unique<VulkanDevice>();
		}
		STOP_EXECUTION_MESSAGE("Unsupported device backend!");
		return nullptr;
	}
	VulkanDevice::VulkanDevice()
	{
		ZoneScoped;

		Initialize();
	}

	VulkanDevice::~VulkanDevice()
	{
		ZoneScoped;

		Cleanup();
	}

	void VulkanDevice::Initialize()
	{
		ZoneScoped;

		CreateInstance();
		CreatePhysicalDevice();
		CreateDevice();
		CreateQueues();
		InitializeVMA();
		m_descriptorPoolAllocator = std::make_unique<DescriptorPoolAllocator>(m_device);
		CreateDefaultSampler();

		m_valid = true;
	}

	void VulkanDevice::CreateInstance()
	{
		ZoneScoped;

		// Enable validation layers in debug mode
#ifdef NDEBUG
		m_enableValidationLayers = false;
#else
		m_enableValidationLayers = true;
#endif
		m_enableValidationLayers = m_enableValidationLayers && CheckValidationLayerSupport();

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Rhygine Engine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Rhygine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		// Get required extensions
		auto extensions = GetRequiredExtensions();

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		if (m_enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
			createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_instance));

		if (m_enableValidationLayers)
		{
			SetupDebugMessenger();
		}
	}

	bool VulkanDevice::CheckValidationLayerSupport() const
	{
		ZoneScoped;

		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : VALIDATION_LAYERS)
		{
			bool layerFound = false;
			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}
			if (!layerFound)
			{
				return false;
			}
		}
		return true;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDevice::DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT t_messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT t_messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* t_callbackData,
		void* t_userData)
	{
		ZoneScoped;

		Logger::Level level;
		if (t_messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
			level = Logger::Level::Trace;
		else if (t_messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
			level = Logger::Level::Info;
		else if (t_messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			level = Logger::Level::Warn;
		else if (t_messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			level = Logger::Level::Error;
		else
			level = Logger::Level::Error;

		RHY_LOG(level, t_callbackData->pMessage);

		return VK_FALSE;
	}

	void VulkanDevice::SetupDebugMessenger()
	{
		ZoneScoped;

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr;

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");

		if (func == nullptr || func(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
		{
			STOP_EXECUTION_MESSAGE("failed to set up debug messenger!");
		}

		m_pfnSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(m_instance, "vkSetDebugUtilsObjectNameEXT");
	}

	std::vector<const char*> VulkanDevice::GetRequiredExtensions() const
	{
		ZoneScoped;

		std::vector<const char*> extensions;

		// Add Vulkan surface extension
		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

		// Add platform-specific surface extension(s)
		auto platformExtensions = GetRequiredSurfaceExtensions();
		extensions.insert(extensions.end(), platformExtensions.begin(), platformExtensions.end());

		// Add additional optional instance extensions
		extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

		if (m_enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void VulkanDevice::CreatePhysicalDevice()
	{
		ZoneScoped;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			STOP_EXECUTION_MESSAGE("Failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

		// Score each device and pick the best one
		VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
		uint32_t bestScore = 0;

		for (const auto& device : devices)
		{
			uint32_t score = ScorePhysicalDevice(device);
			if (score > bestScore)
			{
				bestScore = score;
				bestDevice = device;
			}
		}

		if (bestDevice == VK_NULL_HANDLE)
		{
			STOP_EXECUTION_MESSAGE("Failed to find a suitable GPU!");
		}

		m_physicalDevice = bestDevice;

		// Get device name
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(m_physicalDevice, &properties);
		m_deviceName = properties.deviceName;
	}

	uint32_t VulkanDevice::ScorePhysicalDevice(VkPhysicalDevice t_device) const
	{
		ZoneScoped;

		uint32_t score = 0;

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(t_device, &properties);

		// Discrete GPUs get a big score bonus
		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1000;
		}
		else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
		{
			score += 500;
		}

		// Check queue families
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(t_device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(t_device, &queueFamilyCount, queueFamilies.data());

		bool hasGraphicsQueue = false;
		bool hasComputeQueue = false;
		bool hasTransferQueue = false;

		for (uint32_t i = 0; i < queueFamilyCount; i++)
		{
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				hasGraphicsQueue = true;
			}
			if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				hasComputeQueue = true;
			}
			if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				hasTransferQueue = true;
			}
		}

		if (!hasGraphicsQueue)
		{
			return 0;
		}

		// Check for required extensions
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(t_device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(t_device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(REQUIRED_DEVICE_EXTENSIONS.begin(), REQUIRED_DEVICE_EXTENSIONS.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		if (!requiredExtensions.empty())
		{
			return 0;
		}

		// Check for Vulkan 1.3 support
		VkPhysicalDeviceVulkan13Properties vulkan13Properties{};
		vulkan13Properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES;

		VkPhysicalDeviceProperties2 props2{};
		props2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		props2.pNext = &vulkan13Properties;
		vkGetPhysicalDeviceProperties2(t_device, &props2);

		// Check for dynamic rendering support
		VkPhysicalDeviceVulkan13Features vulkan13Features{};
		vulkan13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		vulkan13Features.dynamicRendering = VK_TRUE;

		VkPhysicalDeviceFeatures2 features2{};
		features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		features2.pNext = &vulkan13Features;
		vkGetPhysicalDeviceFeatures2(t_device, &features2);

		if (!vulkan13Features.dynamicRendering)
		{
			return 0;
		}

		return score;
	}

	void VulkanDevice::CreateDevice()
	{
		ZoneScoped;

		// Get queue families
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

		// Create queues
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> computeFamily;
		std::optional<uint32_t> transferFamily;

		// Find queue families
		for (uint32_t i = 0; i < queueFamilyCount; i++)
		{
			const auto flags = queueFamilies[i].queueFlags;

			if ((flags & VK_QUEUE_GRAPHICS_BIT) && !graphicsFamily.has_value())
			{
				graphicsFamily = i;
			}

			// Prefer a dedicated compute queue (no graphics bit)
			if (flags & VK_QUEUE_COMPUTE_BIT)
			{
				if (!computeFamily.has_value() || (!(flags & VK_QUEUE_GRAPHICS_BIT) && computeFamily == graphicsFamily))
				{
					computeFamily = i;
				}
			}

			// Prefer a dedicated transfer queue (no graphics or compute bit)
			if (flags & VK_QUEUE_TRANSFER_BIT)
			{
				if (!transferFamily.has_value() || (!(flags & VK_QUEUE_GRAPHICS_BIT) && !(flags & VK_QUEUE_COMPUTE_BIT)))
				{
					transferFamily = i;
				}
			}
		}

		if (!computeFamily.has_value()) computeFamily = graphicsFamily;
		if (!transferFamily.has_value()) transferFamily = graphicsFamily;

		m_graphicsQueueFamilyIndex = graphicsFamily.value();
		m_computeQueueFamilyIndex = computeFamily.value();
		m_transferQueueFamilyIndex = transferFamily.value();

		// Collect unique queue families
		std::set<uint32_t> uniqueFamilies = { m_graphicsQueueFamilyIndex, m_computeQueueFamilyIndex, m_transferQueueFamilyIndex };
		float queuePriority = 1.0f;

		for (uint32_t family : uniqueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = family;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// Enable dynamic rendering and synchronization2
		VkPhysicalDeviceVulkan13Features vulkan13Features{};
		vulkan13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		vulkan13Features.dynamicRendering = VK_TRUE;
		vulkan13Features.synchronization2 = VK_TRUE;

		VkPhysicalDeviceFeatures2 deviceFeatures{};
		deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		deviceFeatures.pNext = &vulkan13Features;

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.pEnabledFeatures = nullptr;
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(REQUIRED_DEVICE_EXTENSIONS.size());
		deviceCreateInfo.ppEnabledExtensionNames = REQUIRED_DEVICE_EXTENSIONS.data();
		deviceCreateInfo.pNext = &deviceFeatures;

		VK_CHECK(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device));
	}

	void VulkanDevice::CreateQueues()
	{
		ZoneScoped;

		vkGetDeviceQueue(m_device, m_graphicsQueueFamilyIndex, 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_device, m_computeQueueFamilyIndex, 0, &m_computeQueue);
		vkGetDeviceQueue(m_device, m_transferQueueFamilyIndex, 0, &m_transferQueue);
	}

	void VulkanDevice::InitializeVMA()
	{
		ZoneScoped;

		VmaAllocatorCreateInfo allocatorInfo{};
		allocatorInfo.physicalDevice = m_physicalDevice;
		allocatorInfo.device = m_device;
		allocatorInfo.instance = m_instance;

		VK_CHECK(vmaCreateAllocator(&allocatorInfo, &m_vmaAllocator));
	}



	void VulkanDevice::CreateDefaultSampler()
	{
		ZoneScoped;

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;

		VK_CHECK(vkCreateSampler(m_device, &samplerInfo, nullptr, &m_defaultSampler));
	}

	void VulkanDevice::Cleanup()
	{
		ZoneScoped;

		if (m_debugMessenger != VK_NULL_HANDLE)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr)
			{
				func(m_instance, m_debugMessenger, nullptr);
			}
		}

		if (m_device != VK_NULL_HANDLE)
		{
			vkDeviceWaitIdle(m_device);

			if (m_defaultSampler != VK_NULL_HANDLE)
			{
				vkDestroySampler(m_device, m_defaultSampler, nullptr);
			}

			m_descriptorPoolAllocator.reset();

			// Destroy VMA allocator
			if (m_vmaAllocator != VK_NULL_HANDLE)
			{
				vmaDestroyAllocator(m_vmaAllocator);
			}

			// Destroy device
			vkDestroyDevice(m_device, nullptr);
		}

		// Destroy instance
		if (m_instance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(m_instance, nullptr);
		}
	}

	// =============================================================================
	// IDevice Implementation
	// =============================================================================

	Buffer* VulkanDevice::CreateBuffer(const BufferDesc& t_desc)
	{
		ZoneScoped;

		auto* buffer = new VulkanBuffer(this, t_desc);
		return buffer;
	}

	void VulkanDevice::DestroyBuffer(Buffer* t_buffer)
	{
		ZoneScoped;

		delete t_buffer;
	}

	Texture* VulkanDevice::CreateTexture(const TextureDesc& t_desc)
	{
		ZoneScoped;

		auto* texture = new VulkanTexture(this, t_desc);
		return texture;
	}

	void VulkanDevice::DestroyTexture(Texture* t_texture)
	{
		ZoneScoped;

		delete t_texture;
	}

	Shader* VulkanDevice::CreateShader(const ShaderDesc& t_desc)
	{
		ZoneScoped;

		auto* shader = new VulkanShader(this, t_desc);
		return shader;
	}

	void VulkanDevice::DestroyShader(Shader* t_shader)
	{
		ZoneScoped;

		delete t_shader;
	}

	Pipeline* VulkanDevice::CreateGraphicsPipeline(const GraphicsPipelineDesc& t_desc)
	{
		ZoneScoped;

		auto* pipeline = new VulkanGraphicsPipeline(this, t_desc);
		return pipeline;
	}

	Pipeline* VulkanDevice::CreateComputePipeline(const ComputePipelineDesc& t_desc)
	{
		ZoneScoped;

		auto* pipeline = new VulkanComputePipeline(this, t_desc);
		return pipeline;
	}

	void VulkanDevice::DestroyPipeline(Pipeline* t_pipeline)
	{
		ZoneScoped;

		delete t_pipeline;
	}

	ResourceLayout* VulkanDevice::CreateResourceLayout(const ResourceLayoutDesc& t_desc)
	{
		ZoneScoped;

		auto* layout = new VulkanResourceLayout(this, t_desc);
		return layout;
	}

	void VulkanDevice::DestroyResourceLayout(ResourceLayout* t_layout)
	{
		ZoneScoped;

		delete t_layout;
	}

	ResourceSet* VulkanDevice::CreateResourceSet(const ResourceSetDesc& t_desc)
	{
		ZoneScoped;

		auto* set = new VulkanResourceSet(this, t_desc);
		return set;
	}

	void VulkanDevice::DestroyResourceSet(ResourceSet* t_set)
	{
		ZoneScoped;

		delete t_set;
	}

	ISwapchain* VulkanDevice::CreateSwapchain(const SwapchainDesc& t_desc)
	{
		ZoneScoped;

		auto* swapchain = new VulkanSwapchain(this, t_desc);
		return static_cast<ISwapchain*>(swapchain);
	}

	void VulkanDevice::DestroySwapchain(ISwapchain* t_swapchain)
	{
		ZoneScoped;

		delete t_swapchain;
	}

	ICommandBuffer* VulkanDevice::GetCommandBuffer(QueueType t_type)
	{
		ZoneScoped;

		switch (t_type)
		{
		case QueueType::Graphics:
			return GetCommandBuffer(m_graphicsQueueFamilyIndex);
		case QueueType::Compute:
			return GetCommandBuffer(m_computeQueueFamilyIndex);
		case QueueType::Transfer:
			return GetCommandBuffer(m_transferQueueFamilyIndex);
		default:
			return GetCommandBuffer(m_graphicsQueueFamilyIndex);
		}
	}

	ICommandBuffer* VulkanDevice::GetCommandBuffer(uint32_t t_queueFamilyIndex)
	{
		ZoneScoped;

		auto* cmdBuffer = new VulkanCommandBuffer(this, t_queueFamilyIndex);
		return cmdBuffer;
	}

	void VulkanDevice::DestroyCommandBuffer(ICommandBuffer* t_buffer)
	{
		ZoneScoped;

		delete t_buffer;
	}

	void VulkanDevice::Submit(const std::vector<ICommandBuffer*>& t_buffers, QueueType t_queueType)
	{
		ZoneScoped;

		Submit(t_buffers, SyncSemaphore{}, SyncSemaphore{}, SyncFence{}, t_queueType);
	}

	void VulkanDevice::Submit(const std::vector<ICommandBuffer*>& t_buffers, SyncSemaphore t_waitSemaphore, SyncSemaphore t_signalSemaphore, SyncFence t_signalFence, QueueType t_queueType)
	{
		ZoneScoped;

		std::vector<VkCommandBuffer> vkBuffers;
		vkBuffers.reserve(t_buffers.size());

		for (auto* buffer : t_buffers)
		{
			auto* vkBuffer = static_cast<VulkanCommandBuffer*>(buffer);
			vkBuffers.push_back(vkBuffer->GetVkCommandBuffer());
		}

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = static_cast<uint32_t>(vkBuffers.size());
		submitInfo.pCommandBuffers = vkBuffers.data();

		VkSemaphore waitSemaphore = VK_NULL_HANDLE;
		VkSemaphore signalSemaphore = VK_NULL_HANDLE;

		VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		switch (t_queueType)
		{
		case QueueType::Graphics:
			waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			break;
		case QueueType::Compute:
			waitStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
			break;
		case QueueType::Transfer:
			waitStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			break;
		default:
			waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}

		if (t_waitSemaphore.nativeHandle)
		{
			waitSemaphore = *static_cast<VkSemaphore*>(t_waitSemaphore.nativeHandle);
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = &waitSemaphore;
			submitInfo.pWaitDstStageMask = &waitStage;
		}

		if (t_signalSemaphore.nativeHandle)
		{
			signalSemaphore = *static_cast<VkSemaphore*>(t_signalSemaphore.nativeHandle);
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &signalSemaphore;
		}

		VkQueue queue = VK_NULL_HANDLE;
		switch (t_queueType)
		{
		case QueueType::Graphics:
			queue = m_graphicsQueue;
			break;
		case QueueType::Compute:
			queue = m_computeQueue;
			break;
		case QueueType::Transfer:
			queue = m_transferQueue;
			break;
		default:
			queue = m_graphicsQueue;
		}

		VkFence fence = VK_NULL_HANDLE;
		if (t_signalFence.nativeHandle)
		{
			fence = *static_cast<VkFence*>(t_signalFence.nativeHandle);
		}

		if (vkQueueSubmit(queue, 1, &submitInfo, fence) != VK_SUCCESS)
		{
			STOP_EXECUTION_MESSAGE("Failed to submit command buffer!");
		}
	}

	void VulkanDevice::WaitIdle()
	{
		ZoneScoped;

		vkDeviceWaitIdle(m_device);
	}

	uint64_t VulkanDevice::GetTotalMemory() const
	{
		ZoneScoped;

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

		uint64_t totalMemory = 0;
		for (uint32_t i = 0; i < memProperties.memoryHeapCount; i++)
		{
			if (memProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
			{
				totalMemory = memProperties.memoryHeaps[i].size;
				break;
			}
		}

		return totalMemory;
	}

	uint64_t VulkanDevice::GetAvailableMemory() const
	{
		ZoneScoped;

		VmaBudget budgets[VK_MAX_MEMORY_HEAPS];
		vmaGetHeapBudgets(m_vmaAllocator, budgets);

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryHeapCount; i++)
		{
			if (memProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
			{
				return budgets[i].budget - budgets[i].usage;
			}
		}
		return 0;
	}

	// =============================================================================
	// Format Mapping
	// =============================================================================

	VkFormat VulkanDevice::GetVkFormat(Format t_format) const
	{
		ZoneScoped;

		static const std::unordered_map<Format, VkFormat> formatMap = {
			{ Format::Undefined, VK_FORMAT_UNDEFINED },
			{ Format::R8_UNORM, VK_FORMAT_R8_UNORM },
			{ Format::R8G8_UNORM, VK_FORMAT_R8G8_UNORM },
			{ Format::R8G8B8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM },
			{ Format::R8G8B8A8_SRGB, VK_FORMAT_R8G8B8A8_SRGB },
			{ Format::B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_UNORM },
			{ Format::B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_SRGB },
			{ Format::R16_FLOAT, VK_FORMAT_R16_SFLOAT },
			{ Format::R16G16_FLOAT, VK_FORMAT_R16G16_SFLOAT },
			{ Format::R16G16_UNORM, VK_FORMAT_R16G16_UNORM },
			{ Format::R16G16B16A16_FLOAT, VK_FORMAT_R16G16B16A16_SFLOAT },
			{ Format::R16G16B16A16_UNORM, VK_FORMAT_R16G16B16A16_UNORM },
			{ Format::R32G32B32A32_FLOAT, VK_FORMAT_R32G32B32A32_SFLOAT },
			{ Format::R32G32B32_FLOAT, VK_FORMAT_R32G32B32_SFLOAT },
			{ Format::R32G32_FLOAT, VK_FORMAT_R32G32_SFLOAT },
			{ Format::R32_FLOAT, VK_FORMAT_R32_SFLOAT },
			{ Format::R64G64B64A64_FLOAT, VK_FORMAT_R64G64B64A64_SFLOAT },
			{ Format::R64G64B64_FLOAT, VK_FORMAT_R64G64B64_SFLOAT },
			{ Format::D16_UNORM, VK_FORMAT_D16_UNORM },
			{ Format::D32_SFLOAT, VK_FORMAT_D32_SFLOAT },
			{ Format::D24_UNORM_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			{ Format::BC1_UNORM, VK_FORMAT_BC1_RGBA_UNORM_BLOCK },
			{ Format::BC1_SRGB, VK_FORMAT_BC1_RGBA_SRGB_BLOCK },
			{ Format::BC2_UNORM, VK_FORMAT_BC2_UNORM_BLOCK },
			{ Format::BC2_SRGB, VK_FORMAT_BC2_SRGB_BLOCK },
			{ Format::BC3_UNORM, VK_FORMAT_BC3_UNORM_BLOCK },
			{ Format::BC3_SRGB, VK_FORMAT_BC3_SRGB_BLOCK },
			{ Format::BC4_UNORM, VK_FORMAT_BC4_UNORM_BLOCK },
			{ Format::BC4_SNORM, VK_FORMAT_BC4_SNORM_BLOCK },
			{ Format::BC5_UNORM, VK_FORMAT_BC5_UNORM_BLOCK },
			{ Format::BC5_SNORM, VK_FORMAT_BC5_SNORM_BLOCK },
			{ Format::BC6H_UF16, VK_FORMAT_BC6H_UFLOAT_BLOCK },
			{ Format::BC6H_SF16, VK_FORMAT_BC6H_SFLOAT_BLOCK },
			{ Format::BC7_UNORM, VK_FORMAT_BC7_UNORM_BLOCK },
			{ Format::BC7_SRGB, VK_FORMAT_BC7_SRGB_BLOCK },
		};

		auto it = formatMap.find(t_format);
		if (it != formatMap.end())
		{
			return it->second;
		}
		return VK_FORMAT_UNDEFINED;
	}

	Format VulkanDevice::GetFormat(VkFormat t_format) const
	{
		ZoneScoped;

		static const std::unordered_map<VkFormat, Format> reverseMap = {
			{ VK_FORMAT_R8_UNORM, Format::R8_UNORM },
			{ VK_FORMAT_R8G8_UNORM, Format::R8G8_UNORM },
			{ VK_FORMAT_R8G8B8A8_UNORM, Format::R8G8B8A8_UNORM },
			{ VK_FORMAT_R8G8B8A8_SRGB, Format::R8G8B8A8_SRGB },
			{ VK_FORMAT_B8G8R8A8_UNORM, Format::B8G8R8A8_UNORM },
			{ VK_FORMAT_B8G8R8A8_SRGB, Format::B8G8R8A8_SRGB },
			{ VK_FORMAT_R16_SFLOAT, Format::R16_FLOAT },
			{ VK_FORMAT_R16G16_SFLOAT, Format::R16G16_FLOAT },
			{ VK_FORMAT_R16G16_UNORM, Format::R16G16_UNORM },
			{ VK_FORMAT_R16G16B16A16_SFLOAT, Format::R16G16B16A16_FLOAT },
			{ VK_FORMAT_R16G16B16A16_UNORM, Format::R16G16B16A16_UNORM },
			{ VK_FORMAT_R32G32B32A32_SFLOAT, Format::R32G32B32A32_FLOAT },
			{ VK_FORMAT_R32G32B32_SFLOAT, Format::R32G32B32_FLOAT },
			{ VK_FORMAT_R32G32_SFLOAT, Format::R32G32_FLOAT },
			{ VK_FORMAT_R32_SFLOAT, Format::R32_FLOAT },
			{ VK_FORMAT_R64G64B64A64_SFLOAT, Format::R64G64B64A64_FLOAT },
			{ VK_FORMAT_R64G64B64_SFLOAT, Format::R64G64B64_FLOAT },
			{ VK_FORMAT_D16_UNORM, Format::D16_UNORM },
			{ VK_FORMAT_D32_SFLOAT, Format::D32_SFLOAT },
			{ VK_FORMAT_D24_UNORM_S8_UINT, Format::D24_UNORM_S8_UINT },
			{ VK_FORMAT_BC1_RGBA_UNORM_BLOCK, Format::BC1_UNORM },
			{ VK_FORMAT_BC1_RGBA_SRGB_BLOCK, Format::BC1_SRGB },
			{ VK_FORMAT_BC2_UNORM_BLOCK, Format::BC2_UNORM },
			{ VK_FORMAT_BC2_SRGB_BLOCK, Format::BC2_SRGB },
			{ VK_FORMAT_BC3_UNORM_BLOCK, Format::BC3_UNORM },
			{ VK_FORMAT_BC3_SRGB_BLOCK, Format::BC3_SRGB },
			{ VK_FORMAT_BC4_UNORM_BLOCK, Format::BC4_UNORM },
			{ VK_FORMAT_BC4_SNORM_BLOCK, Format::BC4_SNORM },
			{ VK_FORMAT_BC5_UNORM_BLOCK, Format::BC5_UNORM },
			{ VK_FORMAT_BC5_SNORM_BLOCK, Format::BC5_SNORM },
			{ VK_FORMAT_BC6H_UFLOAT_BLOCK, Format::BC6H_UF16 },
			{ VK_FORMAT_BC6H_SFLOAT_BLOCK, Format::BC6H_SF16 },
			{ VK_FORMAT_BC7_UNORM_BLOCK, Format::BC7_UNORM },
			{ VK_FORMAT_BC7_SRGB_BLOCK, Format::BC7_SRGB },
		};

		auto it = reverseMap.find(t_format);
		if (it != reverseMap.end())
		{
			return it->second;
		}
		return Format::Undefined;
	}

	// =============================================================================
	// State Mapping
	// =============================================================================

	VkImageLayout VulkanDevice::GetVkImageLayout(ResourceState t_state) const
	{
		ZoneScoped;

		switch (t_state)
		{
		case ResourceState::Undefined:
			return VK_IMAGE_LAYOUT_UNDEFINED;
		case ResourceState::RenderTarget:
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		case ResourceState::DepthWrite:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		case ResourceState::DepthRead:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		case ResourceState::ShaderResource:
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		case ResourceState::UnorderedAccess:
			return VK_IMAGE_LAYOUT_GENERAL;
		case ResourceState::TransferDst:
			return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		case ResourceState::TransferSrc:
			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		case ResourceState::Present:
			return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		default:
			return VK_IMAGE_LAYOUT_UNDEFINED;
		}
	}

	VkAccessFlags VulkanDevice::GetVkAccessFlags(ResourceState t_state) const
	{
		ZoneScoped;

		switch (t_state)
		{
		case ResourceState::Undefined:
			return 0;
		case ResourceState::RenderTarget:
			return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		case ResourceState::DepthWrite:
			return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		case ResourceState::DepthRead:
			return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		case ResourceState::ShaderResource:
			return VK_ACCESS_SHADER_READ_BIT;
		case ResourceState::UnorderedAccess:
			return VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;
		case ResourceState::TransferDst:
			return VK_ACCESS_TRANSFER_WRITE_BIT;
		case ResourceState::TransferSrc:
			return VK_ACCESS_TRANSFER_READ_BIT;
		case ResourceState::Present:
			return VK_ACCESS_MEMORY_READ_BIT;
		default:
			return 0;
		}
	}

	VkPipelineStageFlags VulkanDevice::GetVkPipelineStageFlags(ResourceState t_state) const
	{
		ZoneScoped;

		switch (t_state)
		{
		case ResourceState::Undefined:
			return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		case ResourceState::RenderTarget:
			return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		case ResourceState::DepthWrite:
		case ResourceState::DepthRead:
			return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		case ResourceState::ShaderResource:
		case ResourceState::UnorderedAccess:
			return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		case ResourceState::TransferDst:
		case ResourceState::TransferSrc:
			return VK_PIPELINE_STAGE_TRANSFER_BIT;
		case ResourceState::Present:
			return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		default:
			return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		}
	}

	// =============================================================================
	// Usage Mapping
	// =============================================================================

	VkBufferUsageFlags VulkanDevice::GetVkBufferUsageFlags(BufferUsage t_usage) const
	{
		ZoneScoped;

		VkBufferUsageFlags flags = 0;

		if ((t_usage & BufferUsage::Vertex) != BufferUsage::None)
			flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		if ((t_usage & BufferUsage::Index) != BufferUsage::None)
			flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		if ((t_usage & BufferUsage::Constant) != BufferUsage::None)
			flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		if ((t_usage & BufferUsage::Storage) != BufferUsage::None)
			flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		if ((t_usage & BufferUsage::TransferSrc) != BufferUsage::None)
			flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		if ((t_usage & BufferUsage::TransferDst) != BufferUsage::None)
			flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		if ((t_usage & BufferUsage::Indirect) != BufferUsage::None)
			flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;

		return flags;
	}

	VkImageUsageFlags VulkanDevice::GetVkImageUsageFlags(TextureUsage t_usage) const
	{
		ZoneScoped;

		VkImageUsageFlags flags = 0;

		if ((t_usage & TextureUsage::Sampled) != TextureUsage::None)
			flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
		if ((t_usage & TextureUsage::Storage) != TextureUsage::None)
			flags |= VK_IMAGE_USAGE_STORAGE_BIT;
		if ((t_usage & TextureUsage::RenderTarget) != TextureUsage::None)
			flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		if ((t_usage & TextureUsage::DepthStencil) != TextureUsage::None)
			flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		if ((t_usage & TextureUsage::TransferSrc) != TextureUsage::None)
			flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		if ((t_usage & TextureUsage::TransferDst) != TextureUsage::None)
			flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		return flags;
	}

	VkPipelineStageFlags2 VulkanDevice::GetVkPipelineStageFlags2(ResourceState t_state) const
	{
		ZoneScoped;

		switch (t_state)
		{
		case ResourceState::Undefined:
			return VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
		case ResourceState::RenderTarget:
			return VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
		case ResourceState::DepthWrite:
		case ResourceState::DepthRead:
			return VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
		case ResourceState::ShaderResource:
		case ResourceState::UnorderedAccess:
			return VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
		case ResourceState::TransferDst:
		case ResourceState::TransferSrc:
			return VK_PIPELINE_STAGE_2_TRANSFER_BIT;
		case ResourceState::Present:
			return VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;
		case ResourceState::VertexBuffer:
		case ResourceState::IndexBuffer:
			return VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT;
		case ResourceState::ConstantBuffer:
			return VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT;
		case ResourceState::Indirect:
			return VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT;
		default:
			return VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
		}
	}

	VkAccessFlags2 VulkanDevice::GetVkAccessFlags2(ResourceState t_state) const
	{
		ZoneScoped;

		switch (t_state)
		{
		case ResourceState::Undefined:
			return VK_ACCESS_2_NONE;
		case ResourceState::RenderTarget:
			return VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
		case ResourceState::DepthWrite:
			return VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		case ResourceState::DepthRead:
			return VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		case ResourceState::ShaderResource:
			return VK_ACCESS_2_SHADER_READ_BIT;
		case ResourceState::UnorderedAccess:
			return VK_ACCESS_2_SHADER_READ_BIT | VK_ACCESS_2_SHADER_WRITE_BIT;
		case ResourceState::TransferDst:
			return VK_ACCESS_2_TRANSFER_WRITE_BIT;
		case ResourceState::TransferSrc:
			return VK_ACCESS_2_TRANSFER_READ_BIT;
		case ResourceState::Present:
			return VK_ACCESS_2_MEMORY_READ_BIT;
		case ResourceState::VertexBuffer:
			return VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT;
		case ResourceState::IndexBuffer:
			return VK_ACCESS_2_INDEX_READ_BIT;
		case ResourceState::ConstantBuffer:
			return VK_ACCESS_2_UNIFORM_READ_BIT;
		case ResourceState::Indirect:
			return VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT;
		default:
			return VK_ACCESS_2_NONE;
		}
	}

	VkShaderStageFlags VulkanDevice::GetVkShaderStageFlags(ShaderStage t_stage) const
	{
		ZoneScoped;

		VkShaderStageFlags flags = 0;
		if ((t_stage & ShaderStage::Vertex) != ShaderStage::None)
			flags |= VK_SHADER_STAGE_VERTEX_BIT;
		if ((t_stage & ShaderStage::Pixel) != ShaderStage::None)
			flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
		if ((t_stage & ShaderStage::Compute) != ShaderStage::None)
			flags |= VK_SHADER_STAGE_COMPUTE_BIT;
		return flags;
	}

	void VulkanDevice::SetDebugName(VkObjectType t_objectType, uint64_t t_objectHandle, const std::string& t_name) const
	{
		ZoneScoped;

		if (!m_enableValidationLayers || t_name.empty() || t_objectHandle == 0) return;

		VkDebugUtilsObjectNameInfoEXT nameInfo{};
		nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.objectType = t_objectType;
		nameInfo.objectHandle = t_objectHandle;
		nameInfo.pObjectName = t_name.c_str();

		if (m_pfnSetDebugUtilsObjectNameEXT)
		{
			m_pfnSetDebugUtilsObjectNameEXT(m_device, &nameInfo);
		}
	}

	QueueType VulkanDevice::GetQueueTypeFromFamilyIndex(uint32_t t_familyIndex) const
	{
		ZoneScoped;

		if (t_familyIndex == m_graphicsQueueFamilyIndex)
			return QueueType::Graphics;
		if (t_familyIndex == m_computeQueueFamilyIndex)
			return QueueType::Compute;
		if (t_familyIndex == m_transferQueueFamilyIndex)
			return QueueType::Transfer;
		return QueueType::Graphics;
	}
}