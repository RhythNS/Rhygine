#pragma once

#include <vector>
#include <cstdint>
#include "DataTypes/Concurrency/Mutex.h"

namespace Rhygine
{
	class IDevice;
	class Buffer;
	class Texture;
	class ResourceSet;
	class Pipeline;

	class DeletionQueue
	{
	public:
		void Push(Buffer* t_buffer, uint64_t t_frameIndex);
		void Push(Texture* t_texture, uint64_t t_frameIndex);
		void Push(ResourceSet* t_set, uint64_t t_frameIndex);
		void Push(Pipeline* t_pipeline, uint64_t t_frameIndex);

		void Flush(uint64_t t_completedFrameIndex, IDevice* t_device);

		[[nodiscard]] size_t GetPendingCount() const;

	private:
		struct PendingDeletion
		{
			enum class Type { Buffer, Texture, ResourceSet, Pipeline } type;
			void* resource = nullptr;
			uint64_t frameIndex = 0;
		};

		std::vector<PendingDeletion> m_pending;
		mutable Mutex m_mutex;
	};
}
