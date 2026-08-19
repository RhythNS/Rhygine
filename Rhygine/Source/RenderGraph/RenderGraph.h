#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <cstdint>

#include "RenderGraphTypes.h"
#include "RHIEnums.h"
#include "RHIDescriptors.h"
#include "RenderGraphBuilder.h"
#include "Debug/RenderGraphDebugInfo.h"

namespace Rhygine
{
	class Texture;
	class Buffer;
	class IDevice;
	class ICommandBuffer;
	class ResourceManager;
	class RenderGraphResources;
	class TransientResourcePool;
	class RenderGraphCompiler;

	// =============================================================================
	// Internal Resource and Pass Tracking Structures
	// =============================================================================

	struct RGResourceEntry
	{
		std::string name;
		bool isImported = false;
		bool isTexture = true;

		TextureDesc textureDesc;
		BufferDesc  bufferDesc;

		Texture* physicalTexture = nullptr;
		Buffer*  physicalBuffer  = nullptr;

		uint32_t firstPassIndex = UINT32_MAX;
		uint32_t lastPassIndex  = 0;
		uint32_t refCount = 0;

		ResourceState currentState = ResourceState::Undefined;
	};

	struct RGPassResourceAccess
	{
		uint32_t resourceIndex = 0;
		ResourceState state = ResourceState::Undefined;
		bool isWrite = false;
	};

	struct RGPassEntry
	{
		std::string name;
		RGPassType type = RGPassType::Graphics;
		uint32_t passIndex = 0;

		std::vector<RGPassResourceAccess> reads;
		std::vector<RGPassResourceAccess> writes;

		bool hasSideEffects = false;
		bool isCulled = false;

		std::vector<TextureBarrier> textureBarriers;
		std::vector<BufferBarrier>  bufferBarriers;

		std::function<void(RenderGraphResources&, ICommandBuffer*)> executeCallback;
	};

	// =============================================================================
	// RenderGraph Class Definition
	// =============================================================================

	class RenderGraph
	{
	public:
		RenderGraph();
		~RenderGraph();

		RGTextureHandle Import(const std::string& t_name, Texture* t_texture);
		RGBufferHandle Import(const std::string& t_name, Buffer* t_buffer);

		RGTextureHandle CreateTexture(const std::string& t_name, const TextureDesc& t_desc);
		RGBufferHandle CreateBuffer(const std::string& t_name, const BufferDesc& t_desc);

		template <typename DataT, typename SetupFn, typename ExecuteFn>
		void AddPass(const std::string& t_name, RGPassType t_type, SetupFn t_setup, ExecuteFn t_execute);

		void Compile(IDevice* t_device);
		void Execute(IDevice* t_device);

		void BeginFrame();
		void EndFrame();

		void InvalidateTransients();

		void ExecuteGPUUploads(ResourceManager& t_rm, IDevice& t_device);

		[[nodiscard]] ICommandBuffer* GetRecordedCommandBuffer() const { return m_recordedCommandBuffer; }

		[[nodiscard]] const std::vector<uint32_t>& GetExecutionOrder() const { return m_executionOrder; }
		[[nodiscard]] const std::vector<RGPassEntry>& GetPasses() const { return m_passes; }
		[[nodiscard]] const std::vector<RGResourceEntry>& GetResources() const { return m_resources; }
		[[nodiscard]] const RenderGraphDebugInfo& GetDebugInfo() const { return m_debugInfo; }
		void ExportGraphviz(const std::string& t_path) const;

	private:
		std::vector<RGResourceEntry> m_resources;
		std::vector<RGPassEntry> m_passes;
		std::vector<uint32_t> m_executionOrder;

		std::unique_ptr<TransientResourcePool> m_transientPool;
		bool m_transientsInvalidated = false;

		ICommandBuffer* m_recordedCommandBuffer = nullptr;

		RenderGraphDebugInfo m_debugInfo;

		friend class RenderGraphBuilder;
		friend class RenderGraphCompiler;
		friend class RenderGraphResources;
		friend class RenderGraphVisualizer;
	};

	// =============================================================================
	// Template Implementations
	// =============================================================================

	template <typename DataT, typename SetupFn, typename ExecuteFn>
	void RenderGraph::AddPass(const std::string& t_name, RGPassType t_type, SetupFn t_setup, ExecuteFn t_execute)
	{
		uint32_t passIndex = static_cast<uint32_t>(m_passes.size());
		m_passes.emplace_back();
		RGPassEntry& pass = m_passes.back();
		pass.name = t_name;
		pass.type = t_type;
		pass.passIndex = passIndex;

		auto data = std::make_shared<DataT>();

		RenderGraphBuilder builder(*this, passIndex);

		t_setup(builder, *data);

		pass.hasSideEffects = builder.HasSideEffectsFlag();

		pass.executeCallback = [data, t_execute](RenderGraphResources& res, ICommandBuffer* cmd) {
			t_execute(*data, res, cmd);
		};
	}
}
