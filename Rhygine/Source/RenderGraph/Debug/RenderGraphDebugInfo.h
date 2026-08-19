#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "RenderGraphTypes.h"

namespace Rhygine
{
	struct PassTimingInfo
	{
		std::string name;
		RGPassType type = RGPassType::Graphics;
		float gpuTimeMs = 0.0f;
		float cpuRecordTimeMs = 0.0f;
		uint32_t barrierCount = 0;
		uint32_t drawCallCount = 0;
		uint32_t dispatchCount = 0;
	};

	struct ResourceLifetimeInfo
	{
		std::string name;
		bool isTransient = false;
		uint32_t firstPassIndex = 0;
		uint32_t lastPassIndex = 0;
		uint32_t aliasGroupId = 0;
		size_t physicalSizeBytes = 0;
	};

	struct RenderGraphDebugInfo
	{
		float totalGpuTimeMs = 0.0f;
		float compileTimeMs = 0.0f;
		float barrierGenerationTimeMs = 0.0f;

		std::vector<PassTimingInfo> passes;

		uint32_t transientTextureCount = 0;
		uint32_t transientBufferCount = 0;
		uint32_t importedResourceCount = 0;
		size_t peakTransientMemoryBytes = 0;
		size_t aliasedMemorySavedBytes = 0;

		std::vector<ResourceLifetimeInfo> resources;
	};
}
