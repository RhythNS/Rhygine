#pragma once

#include <cstdint>
#include <string>
#include "World/Resource.h"
#include "VertexLayout.h"
#include "RHIDescriptors.h"

namespace Rhygine
{
	class Buffer;
	class SceneRenderer;

	class Mesh : public Resource
	{
	public:
		Mesh() = default;
		Mesh(Buffer* t_vertexBuffer, Buffer* t_indexBuffer, uint32_t t_indexCount,
			const VertexLayout& t_layout, PrimitiveTopology t_topology = PrimitiveTopology::TriangleList);
		~Mesh() override = default;

		void LoadCPU(const std::string& t_path) override;
		bool NeedsGPUUpload() const override { return false; }
		void Unload(SceneRenderer& t_sceneRenderer) override;

		[[nodiscard]] Buffer* GetVertexBuffer() const { return m_vertexBuffer; }
		[[nodiscard]] Buffer* GetIndexBuffer() const { return m_indexBuffer; }
		[[nodiscard]] uint32_t GetIndexCount() const { return m_indexCount; }
		[[nodiscard]] const VertexLayout& GetVertexLayout() const { return m_vertexLayout; }
		[[nodiscard]] PrimitiveTopology GetTopology() const { return m_topology; }

		void SetVertexBuffer(Buffer* t_vertexBuffer) { m_vertexBuffer = t_vertexBuffer; }
		void SetIndexBuffer(Buffer* t_indexBuffer) { m_indexBuffer = t_indexBuffer; }
		void SetIndexCount(uint32_t t_indexCount) { m_indexCount = t_indexCount; }
		void SetVertexLayout(const VertexLayout& t_layout) { m_vertexLayout = t_layout; }
		void SetTopology(PrimitiveTopology t_topology) { m_topology = t_topology; }

	private:
		Buffer* m_vertexBuffer = nullptr;
		Buffer* m_indexBuffer = nullptr;
		uint32_t m_indexCount = 0;
		VertexLayout m_vertexLayout;
		PrimitiveTopology m_topology = PrimitiveTopology::TriangleList;
	};
}
