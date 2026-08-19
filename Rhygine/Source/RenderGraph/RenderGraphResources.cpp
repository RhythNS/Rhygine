#include "RenderGraphResources.h"

#include <cassert>

#include "RenderGraph.h"

namespace Rhygine
{
	RenderGraphResources::RenderGraphResources(const RenderGraph& t_graph)
		: m_graph(t_graph)
	{
	}

	Texture* RenderGraphResources::GetTexture(RGTextureHandle t_handle) const
	{
		assert(t_handle < m_graph.m_resources.size());
		assert(m_graph.m_resources[t_handle].isTexture);
		assert(m_graph.m_resources[t_handle].physicalTexture != nullptr);
		return m_graph.m_resources[t_handle].physicalTexture;
	}

	Buffer* RenderGraphResources::GetBuffer(RGBufferHandle t_handle) const
	{
		assert(t_handle < m_graph.m_resources.size());
		assert(!m_graph.m_resources[t_handle].isTexture);
		assert(m_graph.m_resources[t_handle].physicalBuffer != nullptr);
		return m_graph.m_resources[t_handle].physicalBuffer;
	}
}
