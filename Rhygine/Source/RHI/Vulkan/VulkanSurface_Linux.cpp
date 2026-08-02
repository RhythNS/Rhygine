#include "VulkanSurface.h"

#include <vulkan/vulkan.h>
#include <tracy/Tracy.hpp>

#include "Debug/Error.h"
#include "Systems/Window.h"

namespace Rhygine
{
	std::vector<const char*> GetRequiredSurfaceExtensions()
	{
		ZoneScoped;

		// TODO: Add VK_KHR_XLIB_SURFACE_EXTENSION_NAME, VK_KHR_XCB_SURFACE_EXTENSION_NAME,
		//       VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME, or VK_EXT_ACQUIRE_XLIB_DISPLAY_EXTENSION_NAME
		//       depending on the chosen Linux backend.
		return {};
	}

	VkSurfaceKHR CreateVulkanSurface(VkInstance t_instance, const Window* t_window)
	{
		ZoneScoped;

		(void)t_instance;
		(void)t_window;
		STOP_EXECUTION_MESSAGE("Vulkan surface creation not implemented on Linux!");
		return VK_NULL_HANDLE;
	}
}
