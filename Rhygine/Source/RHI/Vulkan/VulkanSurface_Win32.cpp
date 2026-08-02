#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#define VK_USE_PLATFORM_WIN32_KHR
#endif

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

		return { VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
	}

	VkSurfaceKHR CreateVulkanSurface(VkInstance t_instance, const Window* t_window)
	{
		ZoneScoped;

#ifdef _WIN32
		void* nativeHandle = t_window->GetNativeHandle();
		HWND hwnd = static_cast<HWND>(nativeHandle);
		HINSTANCE hinstance = GetModuleHandle(nullptr);

		VkWin32SurfaceCreateInfoKHR surfaceInfo{};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hinstance = hinstance;
		surfaceInfo.hwnd = hwnd;

		VkSurfaceKHR surface = VK_NULL_HANDLE;
		if (vkCreateWin32SurfaceKHR(t_instance, &surfaceInfo, nullptr, &surface) != VK_SUCCESS)
		{
			STOP_EXECUTION_MESSAGE("Failed to create Vulkan Win32 surface!");
		}

		return surface;
#else
		STOP_EXECUTION_MESSAGE("Win32 Vulkan surface creation requested on a non-Windows platform!");
		return VK_NULL_HANDLE;
#endif
	}
}
