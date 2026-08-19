#include "Mesh.h"
#include "SceneRenderer.h"
#include <tracy/Tracy.hpp>

namespace Rhygine
{
	Mesh::Mesh(Buffer* t_vertexBuffer, Buffer* t_indexBuffer, uint32_t t_indexCount,
		const VertexLayout& t_layout, PrimitiveTopology t_topology)
		: m_vertexBuffer(t_vertexBuffer)
		, m_indexBuffer(t_indexBuffer)
		, m_indexCount(t_indexCount)
		, m_vertexLayout(t_layout)
		, m_topology(t_topology)
	{
		ZoneScoped;
		m_state = State::GPUReady;
	}

	void Mesh::LoadCPU(const std::string& /*t_path*/)
	{
		ZoneScoped;
		// Note: Follow-up when glTF / model loading is added
		m_state = State::CPUReady;
	}

	void Mesh::Unload(SceneRenderer& t_sceneRenderer)
	{
		ZoneScoped;
		if (m_vertexBuffer)
		{
			t_sceneRenderer.QueueForDeletion(m_vertexBuffer);
			m_vertexBuffer = nullptr;
		}
		if (m_indexBuffer)
		{
			t_sceneRenderer.QueueForDeletion(m_indexBuffer);
			m_indexBuffer = nullptr;
		}
		m_indexCount = 0;
		m_state = State::Unloaded;
	}
}
