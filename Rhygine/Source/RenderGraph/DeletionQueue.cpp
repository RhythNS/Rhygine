#include "DeletionQueue.h"

#include <algorithm>
#include <cassert>

#include "IDevice.h"
#include "DataTypes/Concurrency/Mutex.h"


namespace Rhygine
{
	void DeletionQueue::Push(Buffer* t_buffer, uint64_t t_frameIndex)
	{
		std::lock_guard<Mutex> lock(m_mutex);
		m_pending.push_back({ PendingDeletion::Type::Buffer, t_buffer, t_frameIndex });
	}

	void DeletionQueue::Push(Texture* t_texture, uint64_t t_frameIndex)
	{
		std::lock_guard<Mutex> lock(m_mutex);
		m_pending.push_back({ PendingDeletion::Type::Texture, t_texture, t_frameIndex });
	}

	void DeletionQueue::Push(ResourceSet* t_set, uint64_t t_frameIndex)
	{
		std::lock_guard<Mutex> lock(m_mutex);
		m_pending.push_back({ PendingDeletion::Type::ResourceSet, t_set, t_frameIndex });
	}

	void DeletionQueue::Push(Pipeline* t_pipeline, uint64_t t_frameIndex)
	{
		std::lock_guard<Mutex> lock(m_mutex);
		m_pending.push_back({ PendingDeletion::Type::Pipeline, t_pipeline, t_frameIndex });
	}

	void DeletionQueue::Flush(uint64_t t_completedFrameIndex, IDevice* t_device)
	{
		std::lock_guard<Mutex> lock(m_mutex);

		std::erase_if(m_pending, [&](const PendingDeletion& entry) {
			if (entry.frameIndex <= t_completedFrameIndex)
			{
				if (t_device)
				{
					switch (entry.type)
					{
					case PendingDeletion::Type::Buffer:
						t_device->DestroyBuffer(static_cast<Buffer*>(entry.resource));
						break;
					case PendingDeletion::Type::Texture:
						t_device->DestroyTexture(static_cast<Texture*>(entry.resource));
						break;
					case PendingDeletion::Type::ResourceSet:
						t_device->DestroyResourceSet(static_cast<ResourceSet*>(entry.resource));
						break;
					case PendingDeletion::Type::Pipeline:
						t_device->DestroyPipeline(static_cast<Pipeline*>(entry.resource));
						break;
					}
				}
				return true;
			}
			return false;
		});
	}

	size_t DeletionQueue::GetPendingCount() const
	{
		std::lock_guard<Mutex> lock(m_mutex);
		return m_pending.size();
	}
}
