#pragma once

#include <string>
#include <memory>

#include "RenderGraphTypes.h"
#include "IDevice.h"

namespace Rhygine
{
	class SceneRenderer;

	class Resource
	{
	public:
		enum class State
		{
			Unloaded,
			Loading,
			CPUReady,
			Staging,
			GPUReady,
			Failed
		};

		struct StagingRequest
		{
			Buffer* stagingBuffer = nullptr;

			enum class TargetType { Texture, Buffer };
			TargetType targetType;

			TextureDesc textureDesc;

			BufferDesc bufferDesc;

			size_t dataSize = 0;
			uint32_t mipLevels = 1;
		};
		
		struct StagingResult
		{
			RGTextureHandle textureHandle = RG_INVALID_TEXTURE;
			RGBufferHandle bufferHandle = RG_INVALID_BUFFER;
		};

		virtual ~Resource() = default;

		virtual void LoadCPU(const std::string& t_path) = 0;

		virtual bool NeedsGPUUpload() const { return false; }

		virtual void CreateStagingRequest(IDevice& t_device, StagingRequest& t_request) { }

		virtual void FinalizeGPU(const StagingResult& t_result) {}

		virtual bool CanFreeCPUData() const { return false; }
		virtual void FreeCPUData() {}

		virtual void Unload(SceneRenderer& t_sceneRenderer) = 0;

		State GetState() const { return m_state; }

	protected:
		State m_state = State::Unloaded;
	};
}
