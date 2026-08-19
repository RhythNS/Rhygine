#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <queue>
#include <tuple>

#include "Debug/Error.h"
#include "Resource.h"
#include "ResourceHandle.h"
#include "ResourceEntry.h"
#include "DataTypes/Concurrency/ThreadPool.h"
#include "DataTypes/Concurrency/Mutex.h"

namespace Rhygine
{
	class SceneRenderer;
	class IDevice;

	class ResourceManager
	{
	public:
		struct GPUUploadRequest
		{
			Resource* resource = nullptr;
			Resource::StagingRequest stagingRequest;
		};

		ResourceManager(SceneRenderer& t_sceneRenderer, IDevice& t_device);
		~ResourceManager();

		template <typename T>
		ResourceHandle<T> Load(const std::string& t_path)
		{
			std::unique_lock<Mutex> lock(m_CPUMutex);
			auto it = m_resources.find(t_path);
			if (it != m_resources.end())
			{
				auto entry = std::dynamic_pointer_cast<ResourceEntry<T>>(it->second);
				if (!entry)
				{
					STOP_EXECUTION_MESSAGE("Resource type mismatch for path: " + t_path);
					return ResourceHandle<T>();
				}
				return ResourceHandle<T>(entry);
			}
			else
			{
				auto entry = std::make_shared<ResourceEntry<T>>(t_path);
				m_resources[t_path] = entry;
				lock.unlock();
				// TODO: Use a different thread pool for resource loading
				ThreadPool::GetInstance().Submit([entry, t_path, this]()
					{
						entry->Get()->LoadCPU(t_path);

						std::unique_lock<Mutex> cpuLock(m_CPUMutex);
						if (!entry->Get()->NeedsGPUUpload() || entry->GetState() == Resource::State::Failed)
						{
							return;
						}

						m_cpuReadyForGPUUpload.push(entry->GetRawResource());
					}, ThreadPool::Priority::Normal);

				return ResourceHandle<T>(entry);
			}
		}

		template <typename T>
		ResourceHandle<T> Get(const std::string& t_path)
		{
			std::unique_lock<Mutex> lock(m_CPUMutex);
			auto it = m_resources.find(t_path);
			if (it != m_resources.end())
			{
				auto entry = std::dynamic_pointer_cast<ResourceEntry<T>>(it->second);
				if (!entry)
				{
					STOP_EXECUTION_MESSAGE("Resource type mismatch for path: " + t_path);
					return ResourceHandle<T>();
				}
				return ResourceHandle<T>(entry);
			}

			return ResourceHandle<T>();
		}

		void CollectUnused();
		void Unload(const std::string& t_path);

		void ProcessStagingRequests();

		std::queue<std::unique_ptr<GPUUploadRequest>> DrainStagingRequests();

	private:
		static constexpr size_t USE_COUNT_FOR_UNLOAD = 1;

		IDevice& m_device;
		SceneRenderer& m_sceneRenderer;
		Mutex m_CPUMutex;
		Mutex m_GPUMutex;
		std::unordered_map<std::string, std::shared_ptr<ResourceEntryBase>> m_resources;
		std::queue<Resource*> m_cpuReadyForGPUUpload;
		std::queue<std::unique_ptr<GPUUploadRequest>> m_pendingUploads;
	};
}
