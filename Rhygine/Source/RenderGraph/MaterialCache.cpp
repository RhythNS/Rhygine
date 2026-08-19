#include "MaterialCache.h"

namespace Rhygine
{
	MaterialCache::MaterialCache(IDevice* t_device)
		: m_device(t_device)
	{
	}

	Material* MaterialCache::GetOrCreate(const MaterialDefinition* t_def)
	{
		std::lock_guard<Mutex> lock(m_mutex);

		auto it = m_cache.find(t_def);
		if (it != m_cache.end())
		{
			return it->second.get();
		}

		// TODO: Shaders compilation and pipeline creation will be hooked up in Phase 7 / Material System.
		return nullptr;
	}

	void MaterialCache::InvalidateAll()
	{
		std::lock_guard<Mutex> lock(m_mutex);
		m_cache.clear();
	}

	void MaterialCache::ProcessPendingInvalidations()
	{
		// TODO: Implement this function to handle any pending invalidations if needed in the future.
	}
}
