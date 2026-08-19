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
        MaterialCache(IDevice* t_device = nullptr);

        void SetDevice(IDevice* t_device) { m_device = t_device; }
        [[nodiscard]] IDevice* GetDevice() const { return m_device; }

        Material* GetOrCreate(const MaterialDefinition* t_def);

        Material* RegisterMaterial(const MaterialDefinition* t_def, std::unique_ptr<Material> t_material)
        {
            std::lock_guard<Mutex> lock(m_mutex);
            Material* ptr = t_material.get();
            m_cache[t_def] = std::move(t_material);
            return ptr;
        }

        [[nodiscard]] size_t GetCachedCount()
        {
            std::lock_guard<Mutex> lock(m_mutex);
            return m_cache.size();
        }

        void InvalidateAll();

        void ProcessPendingInvalidations();

    private:
        IDevice* m_device;
        std::unordered_map<const MaterialDefinition*, std::unique_ptr<Material>> m_cache;

        Mutex m_mutex;
	};
}
