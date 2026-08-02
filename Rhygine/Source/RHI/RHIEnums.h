#pragma once
#include <cstdint>

namespace Rhygine
{
	// =============================================================================
	// Format Enums
	// =============================================================================
	enum class Format : uint8_t
	{
		Undefined = 0,

		// 8-bit formats
		R8G8B8A8_UNORM,
		R8G8B8A8_SRGB,
		B8G8R8A8_UNORM,
		B8G8R8A8_SRGB,
		R8_UNORM,
		R8G8_UNORM,

		// 16-bit formats
		R16G16B16A16_FLOAT,
		R16G16B16A16_UNORM,
		R16G16_FLOAT,
		R16G16_UNORM,
		R16_FLOAT,
		D16_UNORM,

		// 32-bit formats
		R32G32B32A32_FLOAT,
		R32G32B32_FLOAT,
		R32G32_FLOAT,
		R32_FLOAT,
		D32_SFLOAT,
		D24_UNORM_S8_UINT,

		// 64-bit formats
		R64G64B64A64_FLOAT,
		R64G64B64_FLOAT,

		// Block compressed formats
		BC1_UNORM,
		BC1_SRGB,
		BC2_UNORM,
		BC2_SRGB,
		BC3_UNORM,
		BC3_SRGB,
		BC4_UNORM,
		BC4_SNORM,
		BC5_UNORM,
		BC5_SNORM,
		BC6H_UF16,
		BC6H_SF16,
		BC7_UNORM,
		BC7_SRGB,
	};

	// =============================================================================
	// Texture Usage Flags
	// =============================================================================
	enum class TextureUsage : uint8_t
	{
		None = 0,
		Sampled = 1 << 0,       // Shader Resource View (SRV)
		Storage = 1 << 1,       // Unordered Access View (UAV)
		RenderTarget = 1 << 2,  // Render Target View (RTV)
		DepthStencil = 1 << 3,  // Depth Stencil View (DSV)
		TransferSrc = 1 << 4,   // Transfer source (copy from)
		TransferDst = 1 << 5,   // Transfer destination (copy to)
	};

	inline TextureUsage operator|(TextureUsage a, TextureUsage b)
	{
		return static_cast<TextureUsage>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline TextureUsage operator&(TextureUsage a, TextureUsage b)
	{
		return static_cast<TextureUsage>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	// =============================================================================
	// Buffer Usage Flags
	// =============================================================================
	enum class BufferUsage : uint8_t
	{
		None = 0,
		Vertex = 1 << 0,        // Vertex buffer
		Index = 1 << 1,         // Index buffer
		Constant = 1 << 2,      // Constant buffer (UBO)
		Storage = 1 << 3,       // Storage buffer (SSBO/UAV)
		TransferSrc = 1 << 4,   // Transfer source
		TransferDst = 1 << 5,   // Transfer destination
		Indirect = 1 << 6,      // Indirect draw buffer
	};

	inline BufferUsage operator|(BufferUsage a, BufferUsage b)
	{
		return static_cast<BufferUsage>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline BufferUsage operator&(BufferUsage a, BufferUsage b)
	{
		return static_cast<BufferUsage>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	// =============================================================================
	// Shader Stage
	// =============================================================================
	enum class ShaderStage : uint8_t
	{
		None = 0,
		Vertex = 1 << 0,
		Pixel = 1 << 1,
		Compute = 1 << 2,
		AllGraphics = Vertex | Pixel,
		All = Vertex | Pixel | Compute,
	};

	inline ShaderStage operator|(ShaderStage a, ShaderStage b)
	{
		return static_cast<ShaderStage>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline ShaderStage operator&(ShaderStage a, ShaderStage b)
	{
		return static_cast<ShaderStage>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	// =============================================================================
	// Attachment Load Operation
	// =============================================================================
	enum class AttachmentLoadOp : uint8_t
	{
		Load,    // Preserve existing contents
		Clear,   // Clear to a clear value
		DontCare // Contents are undefined (fastest)
	};

	// =============================================================================
	// Present Mode
	// =============================================================================
	enum class PresentMode : uint8_t
	{
		VSync,       // FIFO — guaranteed available
		Mailbox,     // Triple-buffered, uncapped FPS, low latency
		Immediate,   // No sync, may tear
	};

	// =============================================================================
	// Resource State (for synchronization)
	// =============================================================================
	enum class ResourceState : uint8_t
	{
		Undefined = 0,
		RenderTarget,       // Render target write
		DepthWrite,         // Depth stencil write
		DepthRead,          // Depth stencil read
		ShaderResource,     // Shader read
		UnorderedAccess,    // Shader read/write (UAV)
		TransferDst,        // Copy destination
		TransferSrc,        // Copy source
		Present,            // Presentation
		VertexBuffer,       // Vertex buffer binding
		IndexBuffer,        // Index buffer binding
		ConstantBuffer,     // Constant buffer binding
		Indirect,           // Indirect buffer
	};

	// =============================================================================
	// Queue Type
	// =============================================================================
	enum class QueueType : uint8_t
	{
		Graphics,
		Compute,
		Transfer,
	};

	// =============================================================================
	// Index Format
	// =============================================================================
	enum class IndexFormat : uint8_t
	{
		Uint16,
		Uint32,
	};

	// =============================================================================
	// Primitive Topology
	// =============================================================================
	enum class PrimitiveTopology : uint8_t
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,
		TriangleFan,
		PatchList,
	};

	// =============================================================================
	// Fill Mode (Rasterizer)
	// =============================================================================
	enum class FillMode : uint8_t
	{
		Point,
		Wireframe,
		Solid,
	};

	// =============================================================================
	// Cull Mode (Rasterizer)
	// =============================================================================
	enum class CullMode : uint8_t
	{
		None,
		Front,
		Back,
	};

	// =============================================================================
	// Blend Operation
	// =============================================================================
	enum class BlendOp : uint8_t
	{
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max,
	};

	// =============================================================================
	// Blend Factor
	// =============================================================================
	enum class BlendFactor : uint8_t
	{
		Zero,
		One,
		SrcColor,
		InvSrcColor,
		DstColor,
		InvDstColor,
		SrcAlpha,
		InvSrcAlpha,
		DstAlpha,
		InvDstAlpha,
		BlendColor,
		InvBlendColor,
		SrcAlphaSaturate,
	};

	// =============================================================================
	// Comparison Function
	// =============================================================================
	enum class ComparisonFunc : uint8_t
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always,
	};

	// =============================================================================
	// Stencil Operation
	// =============================================================================
	enum class StencilOp : uint8_t
	{
		Keep,
		Zero,
		Replace,
		IncrementClamp,
		DecrementClamp,
		Invert,
		IncrementWrap,
		DecrementWrap,
	};

	// =============================================================================
	// Texture Type
	// =============================================================================
	enum class TextureType : uint8_t
	{
		Texture1D,
		Texture2D,
		Texture3D,
		Cube,
	};

	// =============================================================================
	// Texture Aspect
	// =============================================================================
	enum class TextureAspect : uint8_t
	{
		Color,
		Depth,
		Stencil,
		DepthStencil,
	};

	// =============================================================================
	// Sample Count
	// =============================================================================
	enum class SampleCount : uint8_t
	{
		Count1 = 1,
		Count2 = 2,
		Count4 = 4,
		Count8 = 8,
		Count16 = 16,
		Count32 = 32,
	};

	// =============================================================================
	// Resource Layout Binding Type
	// =============================================================================
	enum class ResourceBindingType : uint8_t
	{
		ConstantBuffer,
		Texture,
		StorageTexture,
		Buffer,
		Sampler,
	};

	// =============================================================================
	// Descriptor Pool Type (for ResourceSet allocation)
	// =============================================================================
	enum class DescriptorPoolType : uint8_t
	{
		TextureSampled,     // For sampled textures (SRV)
		TextureStorage,     // For storage textures (UAV)
		BufferConstant,     // For constant buffers
		BufferStorage,      // For storage buffers (SSBO)
		Sampler,            // For samplers
	};

	// =============================================================================
	// Device Backend
	// =============================================================================
	enum class DeviceBackend : uint8_t
	{
		Vulkan,
		DirectX12,
		Metal,
	};
}