#pragma once

#include <string>

#include "RenderGraphTypes.h"

namespace Rhygine
{
	class Texture;
	class Buffer;
	class TextureDesc;
	class BufferDesc;
	class IDevice;
	class ResourceManager;
	class RenderGraphDebugInfo;

	class RenderGraph
	{
	public:
		// --- Setup Phase ---

		// Import a persistent external resource into the graph
		RGTextureHandle Import(const std::string& t_name, Texture* t_texture);
		RGBufferHandle  Import(const std::string& t_name, Buffer* t_buffer);

		// Create a transient resource (allocated during Compile)
		RGTextureHandle CreateTexture(const std::string& t_name, const TextureDesc& t_desc);
		RGBufferHandle  CreateBuffer(const std::string& t_name, const BufferDesc& t_desc);

		// Register a pass
		template <typename DataT, typename SetupFn, typename ExecuteFn>
		void AddPass(const std::string& t_name, RGPassType t_type, SetupFn t_setup, ExecuteFn t_execute);

		// --- Compile & Execute ---
		void Compile(IDevice* t_device);
		void Execute(IDevice* t_device);

		// --- Frame lifecycle ---
		void BeginFrame();  // Reset for new frame
		void EndFrame();    // Release transient resources back to pool

		// --- Resize handling ---
		void InvalidateTransients();  // Force re-allocation of all transient resources next Compile

		// --- GPU Upload Integration ---
		void ExecuteGPUUploads(ResourceManager& t_rm, IDevice* t_device);

		// --- Debug ---
		const RenderGraphDebugInfo& GetDebugInfo() const;
		void ExportGraphviz(const std::string& t_path) const;
	};
}
