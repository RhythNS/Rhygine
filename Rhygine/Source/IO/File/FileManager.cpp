#include "FileManager.h"

#include <format>
#include <tracy/Tracy.hpp>

#include "Debug/Error.h"
#include "Debug/Logger.h"

void Rhygine::FileManager::Mount(std::unique_ptr<FileProvider> t_provider)
{
	ZoneScoped;
	std::unique_lock<Rhygine::SharedMutex> lock(m_sharedMutex);

    ASSERT_ERROR_MESSAGE \
    (\
        m_providers.size() == 0 || std::find_if(\
            m_providers.begin(), \
            m_providers.end(), \
            [&t_provider](const std::unique_ptr<FileProvider>& x) \
            { \
                return t_provider->GetName() == x->GetName(); \
            } \
        ) == m_providers.end(), \
        t_provider->GetName() + " has already been mounted!" \
    );

	LOG_INFO(std::format("FileManager::Mount: Mounted file provider '{}'", t_provider->GetName()));
	m_providers.push_back(std::move(t_provider));
}

std::unique_ptr<Rhygine::File> Rhygine::FileManager::Open(const std::string& t_path, FileMode t_fileMode) const
{
	ZoneScoped;
	std::shared_lock<Rhygine::SharedMutex> lock(m_sharedMutex);

	//TODO: Is this desired behaviour or should it be something like provider = t_path.Split(":")[0]?
	for (const auto& provider : m_providers)
	{
		if (provider->Has(t_path))
		{
			LOG_TRACE(std::format("FileManager::Open: Found '{}' in provider '{}'", t_path, provider->GetName()));
			return provider->Open(t_path, t_fileMode);
		}
	}
	LOG_ERROR("Could not find file: " + t_path);
	return nullptr;
}

std::unique_ptr<Rhygine::File> Rhygine::FileManager::Open(const std::string& t_provider, const std::string& t_path, FileMode t_fileMode) const
{
	ZoneScoped;
	std::shared_lock<Rhygine::SharedMutex> lock(m_sharedMutex);

	for (const auto& provider : m_providers)
	{
		if (provider->GetName() == t_provider)
		{
			if (!provider->Has(t_path))
			{
				LOG_ERROR("Could not find file: " + t_path);
				return nullptr;
			}
			LOG_TRACE(std::format("FileManager::Open: Found '{}' in provider '{}'", t_path, t_provider));
			return provider->Open(t_path, t_fileMode);
		}
	}
	LOG_ERROR("Could not find provider: " + t_provider);
	return nullptr;
}

bool Rhygine::FileManager::Has(const std::string& t_path) const
{
	ZoneScoped;
	std::shared_lock<Rhygine::SharedMutex> lock(m_sharedMutex);

	for (const auto& provider : m_providers)
	{
		if (provider->Has(t_path))
		{
			return true;
		}
	}
	return false;
}

bool Rhygine::FileManager::Has(const std::string& t_provider, const std::string& t_path) const
{
	ZoneScoped;
	std::shared_lock<Rhygine::SharedMutex> lock(m_sharedMutex);

	for (const auto& provider : m_providers)
	{
		if (provider->GetName() == t_provider)
		{
			return provider->Has(t_path);
		}
	}
	return false;
}
