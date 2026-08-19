#pragma once

#include <cstdint>
#include "RenderGraphTypes.h"

namespace Rhygine
{
	class RenderGraph;
	struct RenderPacket;

	struct FrameResources
	{
		RGTextureHandle backbuffer = RG_INVALID_TEXTURE;
		RGTextureHandle sceneDepth = RG_INVALID_TEXTURE;
		RGBufferHandle  lightList = RG_INVALID_BUFFER;
	};

	class IPassModule
	{
	public:
		virtual ~IPassModule() = default;

		virtual void AddPasses(
			RenderGraph& t_graph,
			const RenderPacket& t_packet,
			const FrameResources& t_frameResources
		) = 0;
	};
}
