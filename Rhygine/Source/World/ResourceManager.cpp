#include "ResourceManager.h"
#include "Debug/Logger.h"

Rhygine::ResourceManager::ResourceManager(SceneRenderer& t_sceneRenderer, IDevice& t_device)
	: m_sceneRenderer(t_sceneRenderer), m_device(t_device)
{}

Rhygine::ResourceManager::~ResourceManager()
{
	for (auto& pair : m_resources)
	{
		pair.second->Unload();
	}
}

void Rhygine::ResourceManager::CollectUnused()
{
	std::unique_lock<Mutex> cpuLock(m_CPUMutex);
	std::erase_if(m_resources, [this](const auto& pair) {
		if (pair.second.use_count() == USE_COUNT_FOR_UNLOAD)
		{
			pair.second->GetRawResource()->Unload(m_sceneRenderer);
			return true;
		}
		return false;
	});
}

void Rhygine::ResourceManager::Unload(const std::string& t_path)
{
	std::unique_lock<Mutex> cpuLock(m_CPUMutex);
	auto it = m_resources.find(t_path);
	if (it == m_resources.end())
	{
		LOG_INFO("Unload: Resource not found for path: " + t_path);
		return;
	}
	if (it->second.use_count() > USE_COUNT_FOR_UNLOAD)
	{
		LOG_INFO("Unload: Resource is still in use for path: " + t_path);
		return;
	}
	m_resources.erase(it);
	cpuLock.unlock();
	it->second->GetRawResource()->Unload(m_sceneRenderer);
}

void Rhygine::ResourceManager::ProcessStagingRequests()
{
	std::unique_lock<Mutex> gpuLock(m_GPUMutex);
	std::unique_lock<Mutex> cpuLock(m_CPUMutex);

	while (!m_cpuReadyForGPUUpload.empty())
	{
		Resource::StagingRequest stagingRequest{};
		Resource* entry = m_cpuReadyForGPUUpload.front();
		m_cpuReadyForGPUUpload.pop();
		entry->CreateStagingRequest(m_device, stagingRequest);

		m_pendingUploads.push(std::make_unique<GPUUploadRequest>(GPUUploadRequest{ entry, std::move(stagingRequest) }));
	}
}

std::queue<std::unique_ptr<Rhygine::ResourceManager::GPUUploadRequest>> Rhygine::ResourceManager::DrainStagingRequests()
{
	std::unique_lock<Mutex> gpuLock(m_GPUMutex);
	return std::move(m_pendingUploads);
}
