set(RHY_VULKAN_SOURCES
	"VulkanDevice.h"
	"VulkanDevice.cpp"
	"VulkanBuffer.h"
	"VulkanCheck.h"
	"VulkanBuffer.cpp"
	"VulkanTexture.h"
	"VulkanTexture.cpp"
	"VulkanShader.h"
	"VulkanShader.cpp"
	"VulkanPipeline.h"
	"VulkanPipeline.cpp"
	"VulkanResourceLayout.h"
	"VulkanResourceLayout.cpp"
	"VulkanResourceSet.h"
	"VulkanResourceSet.cpp"
	"VulkanSwapchain.h"
	"VulkanSwapchain.cpp"
	"VulkanSurface.h"
	"VulkanCommandBuffer.h"
	"VulkanCommandBuffer.cpp"
	"VulkanMemoryAllocatorImpl.cpp"
)

if(WIN32)
	list(APPEND RHY_VULKAN_SOURCES "VulkanSurface_Win32.cpp")
elseif(UNIX AND NOT APPLE)
	list(APPEND RHY_VULKAN_SOURCES "VulkanSurface_Linux.cpp")
else()
	message(FATAL_ERROR "Unsupported platform for Vulkan backend")
endif()

add_library(
	RhyRHIVulkan
	${RHY_VULKAN_SOURCES}
)

target_include_directories(RhyRHIVulkan PUBLIC 
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_SOURCE_DIR}/Rhygine/Source/RHI"
)

target_link_libraries(RhyRHIVulkan PUBLIC 
	Vulkan::Vulkan
	RhyRHI
	VulkanMemoryAllocator
	volk
)
