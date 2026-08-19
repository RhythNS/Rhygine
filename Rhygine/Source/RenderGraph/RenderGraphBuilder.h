#pragma once

#include "RenderGraphTypes.h"
#include "RHIEnums.h"
#include <string>

namespace Rhygine
{
	struct TextureDesc;
	struct BufferDesc;
	class RenderGraph;

	class RenderGraphBuilder
	{
	public:
		RenderGraphBuilder(RenderGraph& t_graph, uint32_t t_passIndex)
			: m_graph(t_graph), m_passIndex(t_passIndex)
		{
		}

		RGTextureHandle ReadTexture(RGTextureHandle t_handle, ResourceState t_state = ResourceState::ShaderResource);
		RGTextureHandle WriteTexture(RGTextureHandle t_handle, ResourceState t_state = ResourceState::RenderTarget);

		RGBufferHandle ReadBuffer(RGBufferHandle t_handle, ResourceState t_state = ResourceState::ShaderResource);
		RGBufferHandle WriteBuffer(RGBufferHandle t_handle, ResourceState t_state = ResourceState::UnorderedAccess);

		RGTextureHandle CreateTexture(const std::string& t_name, const TextureDesc& t_desc);
		RGBufferHandle  CreateBuffer(const std::string& t_name, const BufferDesc& t_desc);

		void HasSideEffects() { m_hasSideEffects = true; }

		[[nodiscard]] bool HasSideEffectsFlag() const { return m_hasSideEffects; }

	private:
		RenderGraph& m_graph;
		uint32_t m_passIndex;
		bool m_hasSideEffects = false;
	};
}
