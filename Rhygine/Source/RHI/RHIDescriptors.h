#pragma once
#include "RHIEnums.h"

#include <cstdint>
#include <string>
#include <vector>

namespace Rhygine
{
	// Forward declarations
	class Window;
	class Buffer;
	class Texture;
	class Shader;
	class Pipeline;
	class ResourceLayout;
	class ResourceSet;

	// =============================================================================
	// Opaque Synchronisation Handles
	// =============================================================================

	/// Opaque handle wrapping a backend-specific semaphore (e.g. VkSemaphore).
	struct SyncSemaphore
	{
		void* nativeHandle = nullptr;
	};

	/// Opaque handle wrapping a backend-specific fence (e.g. VkFence).
	struct SyncFence
	{
		void* nativeHandle = nullptr;
	};

	// =============================================================================
	// CPU Access Type
	// =============================================================================
	enum class CpuAccess : uint8_t
	{
		None = 0, // GPU Only (Fast VRAM - Default for static Vertex/Index/Storage buffers)
		Write,    // CPU to GPU (Host Visible - For Staging & Dynamic Constant Buffers)
		Read      // GPU to CPU (Host Visible - For GPU Readback / Screenshots / Compute Output)
	};

	// =============================================================================
	// Buffer Descriptor
	// =============================================================================
	struct BufferDesc
	{
		uint64_t size = 0;
		BufferUsage usage = BufferUsage::None;
		CpuAccess cpuAccess = CpuAccess::None;
		ResourceState initialState = ResourceState::Undefined;
		std::string debugName;
	};

	// =============================================================================
	// Texture Descriptor
	// =============================================================================
	struct TextureDesc
	{
		uint32_t width = 0;
		uint32_t height = 1;
		uint32_t depth = 1;
		uint32_t mipLevels = 1;
		uint32_t arrayLayers = 1;
		Format format = Format::Undefined;
		TextureType type = TextureType::Texture2D;
		TextureUsage usage = TextureUsage::None;
		SampleCount sampleCount = SampleCount::Count1;
		TextureAspect aspect = TextureAspect::Color;
		ResourceState initialState = ResourceState::Undefined;
		std::string debugName;
	};

	// =============================================================================
	// Shader Descriptor
	// =============================================================================
	struct ShaderDesc
	{
		const void* bytecode = nullptr;
		uint64_t bytecodeSize = 0;
		std::string entryPointName = "main";
		ShaderStage stage = ShaderStage::Vertex;
		std::string debugName;
	};

	// =============================================================================
	// Blend State
	// =============================================================================
	struct BlendState
	{
		bool enable = false;
		BlendFactor srcColor = BlendFactor::One;
		BlendFactor dstColor = BlendFactor::Zero;
		BlendOp colorOp = BlendOp::Add;
		BlendFactor srcAlpha = BlendFactor::One;
		BlendFactor dstAlpha = BlendFactor::Zero;
		BlendOp alphaOp = BlendOp::Add;
		uint8_t colorWriteMask = 0xF; // RGBA
	};

	// =============================================================================
	// Rasterizer State
	// =============================================================================
	struct RasterizerState
	{
		FillMode fillMode = FillMode::Solid;
		CullMode cullMode = CullMode::Back;
		bool frontCounterClockwise = true;
		int32_t depthBias = 0;
		float depthBiasClamp = 0.0f;
		float slopeScaledDepthBias = 0.0f;
		bool depthClipEnable = true;
		bool scissorEnable = false;
		bool multisampleEnable = false;
	};

	// =============================================================================
	// Depth Stencil State
	// =============================================================================
	struct DepthStencilState
	{
		bool depthEnable = true;
		bool depthWriteEnable = true;
		ComparisonFunc depthFunc = ComparisonFunc::Less;
		bool stencilEnable = false;
		uint8_t stencilReadMask = 0xFF;
		uint8_t stencilWriteMask = 0xFF;
		
		// Front face stencil
		StencilOp frontFaceStencilFailOp = StencilOp::Keep;
		StencilOp frontFaceStencilDepthFailOp = StencilOp::Keep;
		StencilOp frontFaceStencilPassOp = StencilOp::Keep;
		ComparisonFunc frontFaceStencilFunc = ComparisonFunc::Always;
		
		// Back face stencil
		StencilOp backFaceStencilFailOp = StencilOp::Keep;
		StencilOp backFaceStencilDepthFailOp = StencilOp::Keep;
		StencilOp backFaceStencilPassOp = StencilOp::Keep;
		ComparisonFunc backFaceStencilFunc = ComparisonFunc::Always;
	};

	// =============================================================================
	// Vertex Input Attribute
	// =============================================================================
	struct VertexInputAttribute
	{
		uint32_t location = 0;
		uint32_t binding = 0;
		Format format = Format::Undefined;
		uint32_t offset = 0;
		std::string semanticName;
	};

	// =============================================================================
	// Vertex Input Binding
	// =============================================================================
	struct VertexInputBinding
	{
		uint32_t binding = 0;
		uint32_t stride = 0;
		bool perInstance = false;
	};

	// =============================================================================
	// Push Constant Range
	// =============================================================================
	struct PushConstantRange
	{
		ShaderStage stage = ShaderStage::Vertex;
		uint32_t offset = 0;
		uint32_t size = 0;
	};

	// =============================================================================
	// Graphics Pipeline Descriptor
	// =============================================================================
	struct GraphicsPipelineDesc
	{
		Shader* vertexShader = nullptr;
		Shader* pixelShader = nullptr;
		
		BlendState blendState;
		RasterizerState rasterizerState;
		DepthStencilState depthStencilState;
		
		PrimitiveTopology topology = PrimitiveTopology::TriangleList;
		SampleCount sampleCount = SampleCount::Count1;
		
		std::vector<VertexInputAttribute> vertexAttributes;
		std::vector<VertexInputBinding> vertexBindings;
		
		std::vector<Format> renderTargetFormats;
		Format depthStencilFormat = Format::Undefined;
		
		// Resource layouts (descriptor set layouts) for this pipeline
		std::vector<ResourceLayout*> resourceLayouts;
		std::vector<PushConstantRange> pushConstantRanges;
		
		uint32_t subpass = 0;
		std::string debugName;
	};

	// =============================================================================
	// Compute Pipeline Descriptor
	// =============================================================================
	struct ComputePipelineDesc
	{
		Shader* computeShader = nullptr;
		
		// Resource layouts (descriptor set layouts) for this pipeline
		std::vector<ResourceLayout*> resourceLayouts;
		std::vector<PushConstantRange> pushConstantRanges;
		
		std::string debugName;
	};

	// =============================================================================
	// Resource Layout Binding
	// =============================================================================
	struct ResourceLayoutBinding
	{
		uint32_t slot = 0;
		ResourceBindingType type = ResourceBindingType::ConstantBuffer;
		ShaderStage shaderStage = ShaderStage::AllGraphics;
		uint32_t count = 1; // For arrays
	};

	// =============================================================================
	// Resource Layout Descriptor
	// =============================================================================
	struct ResourceLayoutDesc
	{
		std::vector<ResourceLayoutBinding> bindings;
		std::string debugName;
	};

	// =============================================================================
	// Resource Set Descriptor
	// =============================================================================
	struct ResourceSetDesc
	{
		ResourceLayout* layout = nullptr;
		std::string debugName;
	};

	// =============================================================================
	// Swapchain Descriptor
	// =============================================================================
	struct SwapchainDesc
	{
		Window* window = nullptr;
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t minImageCount = 2; // Double buffering by default
		uint32_t imageArrayLayers = 1;
		Format format = Format::B8G8R8A8_UNORM;
		TextureUsage usage = TextureUsage::RenderTarget | TextureUsage::TransferDst;
		SampleCount sampleCount = SampleCount::Count1;
		PresentMode presentMode = PresentMode::VSync;
		std::string debugName;
	};

	// =============================================================================
	// Clear Value (for render pass clear colors)
	// =============================================================================
	struct ClearValue
	{
		union
		{
			struct
			{
				float r, g, b, a;
			} color;
			struct
			{
				float depth;
				uint8_t stencil;
			} depthStencil;
		};
		
		bool isDepthStencil = false;

		ClearValue() : color{ 0.0f, 0.0f, 0.0f, 1.0f }, isDepthStencil(false) {}
		ClearValue(float r, float g, float b, float a = 1.0f) : color{ r, g, b, a }, isDepthStencil(false) {}
		ClearValue(float depth, uint8_t stencil = 0) : depthStencil{ depth, stencil }, isDepthStencil(true) {}
	};

	// =============================================================================
	// Render Pass Begin Info
	// =============================================================================
	struct RenderPassBeginInfo
	{
		std::vector<Texture*> colorTargets;
		Texture* depthTarget = nullptr;
		std::vector<ClearValue> clearValues;
		std::vector<AttachmentLoadOp> colorLoadOps;
		AttachmentLoadOp depthLoadOp = AttachmentLoadOp::Clear;
		uint32_t width = 0;
		uint32_t height = 0;
	};

	// =============================================================================
	// Buffer Barrier
	// =============================================================================
	struct BufferBarrier
	{
		Buffer* buffer = nullptr;
		ResourceState srcState = ResourceState::Undefined;
		ResourceState dstState = ResourceState::Undefined;
		uint64_t offset = 0;
		uint64_t size = 0; // 0 means entire buffer
	};

	// =============================================================================
	// Texture Barrier
	// =============================================================================
	struct TextureBarrier
	{
		Texture* texture = nullptr;
		ResourceState srcState = ResourceState::Undefined;
		ResourceState dstState = ResourceState::Undefined;
		uint32_t baseMipLevel = 0;
		uint32_t levelCount = 0; // 0 means all mip levels
		uint32_t baseArrayLayer = 0;
		uint32_t layerCount = 0; // 0 means all layers
	};

	// =============================================================================
	// Viewport
	// =============================================================================
	struct Viewport
	{
		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
		float minDepth = 0.0f;
		float maxDepth = 1.0f;
	};

	// =============================================================================
	// Scissor Rect
	// =============================================================================
	struct ScissorRect
	{
		int32_t x = 0;
		int32_t y = 0;
		uint32_t width = 0;
		uint32_t height = 0;
	};
}