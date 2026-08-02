#pragma once

#include <memory>
#include <vector>
#include <chrono>

namespace Rhygine
{
	class IDevice;
	class ISwapchain;
	class ICommandBuffer;
	class Shader;
	class Buffer;
	class Pipeline;
	class ResourceLayout;
	class ResourceSet;

	struct Vertex
	{
		float position[3];
		float color[3];
	};

	class TempGfx
	{
	public:
		TempGfx();
		~TempGfx();

		void OnUpdate();

	private:
		std::unique_ptr<IDevice> m_device;
		ISwapchain* m_swap;
		ICommandBuffer* m_commandBuffer;

		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;

		Shader* m_vertexShader;
		Shader* m_pixelShader;

		Buffer* m_vertexBuffer;
		Buffer* m_indexBuffer;
		Buffer* m_constantBuffer;

		ResourceLayout* m_resourceLayout;
		ResourceSet* m_resourceSet;

		Pipeline* m_pipeline;

		std::chrono::high_resolution_clock::time_point m_startTime;
	};
}
