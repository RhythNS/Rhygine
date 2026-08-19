#include "TempGfx.h"

#include <tracy/Tracy.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Window.h"
#include "IDevice.h"
#include "ISwapchain.h"
#include "ICommandBuffer.h"
#include "File/StandardFileProvider.h"
#include "File/FileManager.h"

namespace
{
	std::string FindShadersDirectory()
	{
		namespace fs = std::filesystem;
		std::vector<fs::path> candidates = {
			"Shaders/",
			"bin/Shaders/",
			"../Shaders/",
			"../bin/Shaders/",
			"../../Shaders/",
			"../../bin/Shaders/",
			fs::current_path() / "Shaders",
			fs::current_path() / "bin" / "Shaders"
		};

		for (const auto& candidate : candidates)
		{
			if (fs::exists(candidate / "BasicVert.spv"))
			{
				std::string pathStr = candidate.string();
				if (!pathStr.empty() && pathStr.back() != '/' && pathStr.back() != '\\')
				{
					pathStr += "/";
				}
				return pathStr;
			}
		}

		return "Shaders/";
	}
}

Rhygine::TempGfx::TempGfx(IDevice* t_device) : m_device(t_device)
{
	ZoneScoped;

	m_startTime = std::chrono::high_resolution_clock::now();

	System* system = System::GetInstance();
	Window* window = system->GetWindow(1);

	SwapchainDesc swapDesc{};
	swapDesc.window = window;
	swapDesc.width = window->GetWidth();
	swapDesc.height = window->GetHeight();

	m_swap = m_device->CreateSwapchain(swapDesc);
	m_commandBuffer = m_device->GetCommandBuffer();

	std::string shadersDir = FindShadersDirectory();
	FileManager fm;
	fm.Mount(std::make_unique<StandardFileProvider>("shaders", shadersDir));
	FileMode mode = (FileMode)(FileMode::Read | FileMode::Binary);

	std::unique_ptr<File> vertFile = fm.Open("BasicVert.spv", mode);
	std::vector<char> vertShaderCode = vertFile->ReadAll();

	std::unique_ptr<File> pixelFile = fm.Open("BasicPixel.spv", mode);
	std::vector<char> pixelShaderCode = pixelFile->ReadAll();

	ShaderDesc vertShaderDesc{};
	vertShaderDesc.bytecode = vertShaderCode.data();
	vertShaderDesc.bytecodeSize = vertShaderCode.size();
	vertShaderDesc.stage = ShaderStage::Vertex;
	m_vertexShader = m_device->CreateShader(vertShaderDesc);

	ShaderDesc pixelShaderDesc{};
	pixelShaderDesc.bytecode = pixelShaderCode.data();
	pixelShaderDesc.bytecodeSize = pixelShaderCode.size();
	pixelShaderDesc.stage = ShaderStage::Pixel;
	m_pixelShader = m_device->CreateShader(pixelShaderDesc);

	// Define 24 vertices for a 3D cube with distinct colors per face
	m_vertices = {
		// Front face (+Z, Red)
		{{ -0.5f, -0.5f,  0.5f }, { 1.0f, 0.2f, 0.2f }},
		{{  0.5f, -0.5f,  0.5f }, { 1.0f, 0.2f, 0.2f }},
		{{  0.5f,  0.5f,  0.5f }, { 1.0f, 0.2f, 0.2f }},
		{{ -0.5f,  0.5f,  0.5f }, { 1.0f, 0.2f, 0.2f }},

		// Back face (-Z, Green)
		{{  0.5f, -0.5f, -0.5f }, { 0.2f, 1.0f, 0.2f }},
		{{ -0.5f, -0.5f, -0.5f }, { 0.2f, 1.0f, 0.2f }},
		{{ -0.5f,  0.5f, -0.5f }, { 0.2f, 1.0f, 0.2f }},
		{{  0.5f,  0.5f, -0.5f }, { 0.2f, 1.0f, 0.2f }},

		// Top face (+Y, Blue)
		{{ -0.5f,  0.5f,  0.5f }, { 0.2f, 0.4f, 1.0f }},
		{{  0.5f,  0.5f,  0.5f }, { 0.2f, 0.4f, 1.0f }},
		{{  0.5f,  0.5f, -0.5f }, { 0.2f, 0.4f, 1.0f }},
		{{ -0.5f,  0.5f, -0.5f }, { 0.2f, 0.4f, 1.0f }},

		// Bottom face (-Y, Yellow)
		{{ -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.2f }},
		{{  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.2f }},
		{{  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 0.2f }},
		{{ -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 0.2f }},

		// Right face (+X, Cyan)
		{{  0.5f, -0.5f,  0.5f }, { 0.2f, 1.0f, 1.0f }},
		{{  0.5f, -0.5f, -0.5f }, { 0.2f, 1.0f, 1.0f }},
		{{  0.5f,  0.5f, -0.5f }, { 0.2f, 1.0f, 1.0f }},
		{{  0.5f,  0.5f,  0.5f }, { 0.2f, 1.0f, 1.0f }},

		// Left face (-X, Magenta)
		{{ -0.5f, -0.5f, -0.5f }, { 1.0f, 0.2f, 1.0f }},
		{{ -0.5f, -0.5f,  0.5f }, { 1.0f, 0.2f, 1.0f }},
		{{ -0.5f,  0.5f,  0.5f }, { 1.0f, 0.2f, 1.0f }},
		{{ -0.5f,  0.5f, -0.5f }, { 1.0f, 0.2f, 1.0f }}
	};

	// 36 indices for 12 triangles (6 faces * 2 triangles)
	m_indices = {
		 0,  1,  2,   2,  3,  0, // Front
		 4,  5,  6,   6,  7,  4, // Back
		 8,  9, 10,  10, 11,  8, // Top
		12, 13, 14,  14, 15, 12, // Bottom
		16, 17, 18,  18, 19, 16, // Right
		20, 21, 22,  22, 23, 20  // Left
	};

	BufferDesc vertexBufferDesc{};
	vertexBufferDesc.size = m_vertices.size() * sizeof(Vertex);
	vertexBufferDesc.usage = BufferUsage::Vertex | BufferUsage::TransferDst;
	m_vertexBuffer = m_device->CreateBuffer(vertexBufferDesc);

	BufferDesc indexBufferDesc{};
	indexBufferDesc.size = m_indices.size() * sizeof(uint32_t);
	indexBufferDesc.usage = BufferUsage::Index | BufferUsage::TransferDst;
	m_indexBuffer = m_device->CreateBuffer(indexBufferDesc);

	// CPU Constant Buffer for MVP matrix
	BufferDesc cbDesc{};
	cbDesc.size = sizeof(glm::mat4);
	cbDesc.usage = BufferUsage::Constant;
	cbDesc.cpuAccess = CpuAccess::Write;
	m_constantBuffer = m_device->CreateBuffer(cbDesc);

	// Resource Layout & Resource Set for Constant Buffer
	ResourceLayoutBinding cbBinding{};
	cbBinding.slot = 0;
	cbBinding.type = ResourceBindingType::ConstantBuffer;
	cbBinding.shaderStage = ShaderStage::Vertex;
	cbBinding.count = 1;

	ResourceLayoutDesc layoutDesc{};
	layoutDesc.bindings = { cbBinding };
	m_resourceLayout = m_device->CreateResourceLayout(layoutDesc);

	ResourceSetDesc setDesc{};
	setDesc.layout = m_resourceLayout;
	m_resourceSet = m_device->CreateResourceSet(setDesc);
	m_resourceSet->UpdateBuffer(0, m_constantBuffer);

	VertexInputBinding binding{};
	binding.binding = 0;
	binding.stride = sizeof(Vertex);
	binding.perInstance = false;

	VertexInputAttribute posAttr{};
	posAttr.location = 0;
	posAttr.binding = 0;
	posAttr.format = Format::R32G32B32_FLOAT;
	posAttr.offset = offsetof(Vertex, position);

	VertexInputAttribute colAttr{};
	colAttr.location = 1;
	colAttr.binding = 0;
	colAttr.format = Format::R32G32B32_FLOAT;
	colAttr.offset = offsetof(Vertex, color);

	GraphicsPipelineDesc pipelineDesc{};
	pipelineDesc.vertexShader = m_vertexShader;
	pipelineDesc.pixelShader = m_pixelShader;
	pipelineDesc.vertexBindings = { binding };
	pipelineDesc.vertexAttributes = { posAttr, colAttr };
	pipelineDesc.renderTargetFormats = { m_swap->GetFormat() };
	pipelineDesc.rasterizerState.cullMode = CullMode::Back;
	pipelineDesc.resourceLayouts = { m_resourceLayout };

	m_pipeline = m_device->CreateGraphicsPipeline(pipelineDesc);

	// Upload vertex and index data via staging buffers
	BufferDesc stagingVertexBufferDesc{};
	stagingVertexBufferDesc.size = m_vertices.size() * sizeof(Vertex);
	stagingVertexBufferDesc.usage = BufferUsage::TransferSrc;
	stagingVertexBufferDesc.cpuAccess = CpuAccess::Write;
	Buffer* stagingVertexBuffer = m_device->CreateBuffer(stagingVertexBufferDesc);

	void* mappedData = stagingVertexBuffer->Map();
	if (mappedData)
	{
		memcpy(mappedData, m_vertices.data(), m_vertices.size() * sizeof(Vertex));
		stagingVertexBuffer->Unmap();
	}

	BufferDesc stagingIndexBufferDesc{};
	stagingIndexBufferDesc.size = m_indices.size() * sizeof(uint32_t);
	stagingIndexBufferDesc.usage = BufferUsage::TransferSrc;
	stagingIndexBufferDesc.cpuAccess = CpuAccess::Write;
	Buffer* stagingIndexBuffer = m_device->CreateBuffer(stagingIndexBufferDesc);

	mappedData = stagingIndexBuffer->Map();
	if (mappedData)
	{
		memcpy(mappedData, m_indices.data(), m_indices.size() * sizeof(uint32_t));
		stagingIndexBuffer->Unmap();
	}

	m_commandBuffer->Begin();
	m_commandBuffer->CopyBuffer(stagingVertexBuffer, m_vertexBuffer, m_vertices.size() * sizeof(Vertex));
	m_commandBuffer->CopyBuffer(stagingIndexBuffer, m_indexBuffer, m_indices.size() * sizeof(uint32_t));
	m_commandBuffer->End();

	m_device->Submit({ m_commandBuffer });
	m_device->WaitIdle();

	m_device->DestroyBuffer(stagingVertexBuffer);
	m_device->DestroyBuffer(stagingIndexBuffer);
}

Rhygine::TempGfx::~TempGfx()
{
	ZoneScoped;

	m_device->DestroyResourceSet(m_resourceSet);
	m_device->DestroyResourceLayout(m_resourceLayout);

	m_device->DestroyShader(m_vertexShader);
	m_device->DestroyShader(m_pixelShader);

	m_device->DestroyBuffer(m_vertexBuffer);
	m_device->DestroyBuffer(m_indexBuffer);
	m_device->DestroyBuffer(m_constantBuffer);
	
	m_device->DestroyPipeline(m_pipeline);

	m_device->DestroyCommandBuffer(m_commandBuffer);

	m_device->DestroySwapchain(m_swap);
}

void Rhygine::TempGfx::OnUpdate()
{
	ZoneScoped;

	auto now = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float>(now - m_startTime).count();

	float aspect = (float)m_swap->GetWidth() / (float)m_swap->GetHeight();
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), time * 1.2f, glm::vec3(0.5f, 1.0f, 0.2f));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
	proj[1][1] *= -1.0f;

	glm::mat4 mvp = proj * view * model;

	void* mappedData = m_constantBuffer->Map();
	if (mappedData)
	{
		memcpy(mappedData, &mvp, sizeof(mvp));
		m_constantBuffer->Unmap();
	}

	m_swap->AcquireNextImage();

	Texture* backbuffer = m_swap->GetBackbuffer();

	m_commandBuffer->Begin();

	TextureBarrier undefinedToRenderTarget{};
	undefinedToRenderTarget.texture = backbuffer;
	undefinedToRenderTarget.srcState = ResourceState::Undefined;
	undefinedToRenderTarget.dstState = ResourceState::RenderTarget;
	undefinedToRenderTarget.levelCount = 1;
	undefinedToRenderTarget.layerCount = 1;
	m_commandBuffer->ResourceBarrier({}, { undefinedToRenderTarget });

	RenderPassBeginInfo renderPassInfo{};
	renderPassInfo.colorTargets = { backbuffer };
	renderPassInfo.clearValues = { ClearValue(0.1f, 0.1f, 0.12f, 1.0f) };
	renderPassInfo.width = m_swap->GetWidth();
	renderPassInfo.height = m_swap->GetHeight();

	m_commandBuffer->BeginRenderPass(renderPassInfo);

	m_commandBuffer->BindPipeline(m_pipeline);
	m_commandBuffer->BindResourceSet(0, m_resourceSet);

	Viewport viewport{ 0.0f, 0.0f, (float)m_swap->GetWidth(), (float)m_swap->GetHeight(), 0.0f, 1.0f };
	ScissorRect scissor{ 0, 0, m_swap->GetWidth(), m_swap->GetHeight() };
	m_commandBuffer->SetViewport(viewport);
	m_commandBuffer->SetScissor(scissor);
	m_commandBuffer->BindVertexBuffer(0, m_vertexBuffer, 0);
	m_commandBuffer->BindIndexBuffer(m_indexBuffer, 0, IndexFormat::Uint32);
	m_commandBuffer->DrawIndexed(static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);

	m_commandBuffer->EndRenderPass();

	TextureBarrier renderTargetToPresent{};
	renderTargetToPresent.texture = backbuffer;
	renderTargetToPresent.srcState = ResourceState::RenderTarget;
	renderTargetToPresent.dstState = ResourceState::Present;
	renderTargetToPresent.levelCount = 1;
	renderTargetToPresent.layerCount = 1;
	m_commandBuffer->ResourceBarrier({}, { renderTargetToPresent });

	m_commandBuffer->End();

	m_device->Submit({ m_commandBuffer }, m_swap->GetImageAvailableSemaphore(), m_swap->GetRenderFinishedSemaphore());

	m_swap->Present();

	m_device->WaitIdle();
}
