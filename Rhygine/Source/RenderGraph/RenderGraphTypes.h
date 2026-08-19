#pragma once

#include <cstdint>

namespace Rhygine
{
	typedef uint32_t RGTextureHandle;
	typedef uint32_t RGBufferHandle;

	static constexpr RGTextureHandle RG_INVALID_TEXTURE = UINT32_MAX;
	static constexpr RGBufferHandle  RG_INVALID_BUFFER  = UINT32_MAX;

	enum class RGPassType
	{
		Graphics,
		Compute,
		Transfer
	};
}
