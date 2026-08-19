#pragma once

#include <unordered_map>
#include <memory>

#include "DataTypes/Concurrency.h"
#include "IDevice.h"
#include "MaterialDefinition.h"
#include "Material.h"

namespace Rhygine
{
	class MaterialCache
	{
    public:
        MaterialCache(IDevice* t_device);

        // Returns a ready-to-bind Material (Pipeline + ResourceSet)
        // Creates and caches on first request; returns cached on subsequent
        Material* GetOrCreate(const MaterialDefinition* t_def);

        // Called when shaders are reloaded (hot-reload support)
        void InvalidateAll();

        void ProcessPendingInvalidations();

    private:
        IDevice* m_device;
        std::unordered_map<const MaterialDefinition*, std::unique_ptr<Material>> m_cache;

        Mutex m_mutex; // Protects m_cache for thread-safe GetOrCreate
	};
}
