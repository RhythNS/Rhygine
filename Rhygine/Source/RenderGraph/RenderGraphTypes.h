#pragma once

#include <cstdint>

namespace Rhygine
{
	typedef uint32_t RGTextureHandle;
	typedef uint32_t RGBufferHandle;

	enum class RGPassType
	{
		Graphics,
		Compute,
		Transfer,
		AsyncCompute
	};
}
