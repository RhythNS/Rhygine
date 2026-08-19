#pragma once

#include <string>
#include <memory>

namespace Rhygine
{
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

		virtual ~Resource() = default;

		virtual void LoadCPU(const std::string& t_path) = 0;

		virtual bool NeedsGPUUpload() const { return false; }

		/* TODO: Figure this out
		virtual StagingRequest CreateStagingRequest(IDevice& t_device) { return {}; }

		virtual void FinalizeGPU(const StagingResult& t_result) {}
		*/

		virtual bool CanFreeCPUData() const { return false; }
		virtual void FreeCPUData() {}

		// --- Cleanup ---
		virtual void Unload() = 0;

		State GetState() const { return m_state; }

	protected:
		State m_state = State::Unloaded;
	};
}
