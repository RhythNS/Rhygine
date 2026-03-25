#include "FileManager.h"

#include "Debug/Error.h"

void Rhygine::FileManager::Mount(std::unique_ptr<FileProvider> t_provider)
{
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

	m_providers.push_back(std::move(t_provider));
}

std::unique_ptr<Rhygine::File> Rhygine::FileManager::Open(const std::string& t_path, FileMode t_fileMode) const
{
	std::shared_lock<Rhygine::SharedMutex> lock(m_sharedMutex);

	//TODO: Is this desired behaviour or should it be something like provider = t_path.Split(":")[0]?
	for (const auto& provider : m_providers)
	{
		if (provider->Has(t_path))
		{
			return provider->Open(t_path, t_fileMode);
		}
	}
	LOG_ERROR("Could not find file: " + t_path);
	return nullptr;
}

std::unique_ptr<Rhygine::File> Rhygine::FileManager::Open(const std::string& t_provider, const std::string& t_path, FileMode t_fileMode) const
{
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
			return provider->Open(t_path, t_fileMode);
		}
	}
	LOG_ERROR("Could not find provider: " + t_provider);
	return nullptr;
}

bool Rhygine::FileManager::Has(const std::string& t_path) const
{
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
