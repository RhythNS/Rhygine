#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace Rhygine
{
	class Window;

	std::vector<const char*> GetRequiredSurfaceExtensions();
	VkSurfaceKHR CreateVulkanSurface(VkInstance t_instance, const Window* t_window);
}
