#include "RenderGraphBuilder.h"
#include "RenderGraph.h"
#include <cassert>

namespace Rhygine
{
	RGTextureHandle RenderGraphBuilder::ReadTexture(RGTextureHandle t_handle, ResourceState t_state)
	{
		assert(t_handle < m_graph.m_resources.size());
		assert(m_graph.m_resources[t_handle].isTexture);

		RGPassResourceAccess access;
		access.resourceIndex = t_handle;
		access.state = t_state;
		access.isWrite = false;
		m_graph.m_passes[m_passIndex].reads.push_back(access);
		m_graph.m_resources[t_handle].refCount++;
		return t_handle;
	}

	RGTextureHandle RenderGraphBuilder::WriteTexture(RGTextureHandle t_handle, ResourceState t_state)
	{
		assert(t_handle < m_graph.m_resources.size());
		assert(m_graph.m_resources[t_handle].isTexture);

		RGPassResourceAccess access;
		access.resourceIndex = t_handle;
		access.state = t_state;
		access.isWrite = true;
		m_graph.m_passes[m_passIndex].writes.push_back(access);
		m_graph.m_resources[t_handle].refCount++;
		return t_handle;
	}

	RGBufferHandle RenderGraphBuilder::ReadBuffer(RGBufferHandle t_handle, ResourceState t_state)
	{
		assert(t_handle < m_graph.m_resources.size());
		assert(!m_graph.m_resources[t_handle].isTexture);

		RGPassResourceAccess access;
		access.resourceIndex = t_handle;
		access.state = t_state;
		access.isWrite = false;
		m_graph.m_passes[m_passIndex].reads.push_back(access);
		m_graph.m_resources[t_handle].refCount++;
		return t_handle;
	}

	RGBufferHandle RenderGraphBuilder::WriteBuffer(RGBufferHandle t_handle, ResourceState t_state)
	{
		assert(t_handle < m_graph.m_resources.size());
		assert(!m_graph.m_resources[t_handle].isTexture);

		RGPassResourceAccess access;
		access.resourceIndex = t_handle;
		access.state = t_state;
		access.isWrite = true;
		m_graph.m_passes[m_passIndex].writes.push_back(access);
		m_graph.m_resources[t_handle].refCount++;
		return t_handle;
	}

	RGTextureHandle RenderGraphBuilder::CreateTexture(const std::string& t_name, const TextureDesc& t_desc)
	{
		return m_graph.CreateTexture(t_name, t_desc);
	}

	RGBufferHandle RenderGraphBuilder::CreateBuffer(const std::string& t_name, const BufferDesc& t_desc)
	{
		return m_graph.CreateBuffer(t_name, t_desc);
	}
}
