#pragma once
#include <vulkan/vulkan.h>

#include "Debug/Error.h"
#include "Debug/Logger.h"

#define VK_CHECK(expr) \
	do { \
		VkResult vkResult_ = (expr); \
		if (vkResult_ != VK_SUCCESS) \
		{ \
			LOG_ERROR("Vulkan error " + std::to_string(static_cast<int>(vkResult_)) + " at " + std::string(__FILE__) + ":" + std::to_string(__LINE__)); \
			STOP_EXECUTION_MESSAGE("VK_CHECK failed: " #expr); \
		} \
	} while(0)
