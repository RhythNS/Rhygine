#pragma once

#include "RenderGraphTypes.h"

namespace Rhygine
{
	class Texture;
	class Buffer;
	class RenderGraph;

	class RenderGraphResources
	{
	public:
		RenderGraphResources(const RenderGraph& t_graph);

		[[nodiscard]] Texture* GetTexture(RGTextureHandle t_handle) const;
		[[nodiscard]] Buffer* GetBuffer(RGBufferHandle t_handle) const;

	private:
		const RenderGraph& m_graph;
	};
}
