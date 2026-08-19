#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

#include "DataTypes/Concurrency.h"
#include "IDevice.h"
#include "MaterialDefinition.h"
#include "Material.h"
#include "VertexLayout.h"

namespace Rhygine
{
	class FileManager;

	struct PipelineKey
	{
		const MaterialDefinition* definition = nullptr;
		uint64_t vertexLayoutHash = 0;
		PrimitiveTopology topology = PrimitiveTopology::TriangleList;

		bool operator==(const PipelineKey& other) const
		{
			return definition == other.definition
				&& vertexLayoutHash == other.vertexLayoutHash
				&& topology == other.topology;
		}
	};

	struct PipelineKeyHash
	{
		size_t operator()(const PipelineKey& k) const noexcept
		{
			size_t h = std::hash<const void*>{}(k.definition);
			h ^= std::hash<uint64_t>{}(k.vertexLayoutHash) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
			h ^= std::hash<uint32_t>{}(static_cast<uint32_t>(k.topology)) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
			return h;
		}
	};

	class MaterialCache
	{
	public:
		MaterialCache(IDevice* t_device = nullptr, FileManager* t_fileManager = nullptr);
		~MaterialCache();

		void SetDevice(IDevice* t_device) { m_device = t_device; }
		[[nodiscard]] IDevice* GetDevice() const { return m_device; }

		void SetFileManager(FileManager* t_fileManager) { m_fileManager = t_fileManager; }
		[[nodiscard]] FileManager* GetFileManager() const { return m_fileManager; }

		Pipeline* GetOrCreatePipeline(
			const MaterialDefinition* t_def,
			const VertexLayout& t_layout,
			PrimitiveTopology t_topology = PrimitiveTopology::TriangleList
		);

		Material* GetOrCreate(const MaterialDefinition* t_def);

		Material* RegisterMaterial(const MaterialDefinition* t_def, std::unique_ptr<Material> t_material)
		{
			std::lock_guard<Mutex> lock(m_mutex);
			Material* ptr = t_material.get();
			m_cache[t_def] = std::move(t_material);
			return ptr;
		}

		[[nodiscard]] size_t GetCachedCount() const
		{
			std::lock_guard<Mutex> lock(m_mutex);
			return m_cache.size();
		}

		[[nodiscard]] size_t GetCachedShaderCount() const
		{
			std::lock_guard<Mutex> lock(m_mutex);
			return m_shaderCache.size();
		}

		[[nodiscard]] size_t GetCachedPipelineCount() const
		{
			std::lock_guard<Mutex> lock(m_mutex);
			return m_pipelineCache.size();
		}

		void Invalidate(const MaterialDefinition* t_def);
		void QueueInvalidation(const MaterialDefinition* t_def);
		void ProcessPendingInvalidations();
		void InvalidateAll();

		Shader* GetOrCreateShader(const std::string& path, ShaderStage stage);

	private:
		IDevice* m_device;
		FileManager* m_fileManager;
		std::unordered_map<const MaterialDefinition*, std::unique_ptr<Material>> m_cache;
		std::unordered_map<std::string, Shader*> m_shaderCache;
		std::unordered_map<PipelineKey, Pipeline*, PipelineKeyHash> m_pipelineCache;
		std::vector<const MaterialDefinition*> m_pendingInvalidations;
		uint32_t m_nextMaterialId = 1;

		mutable Mutex m_mutex;

		Pipeline* GetOrCreatePipelineInternal(const MaterialDefinition* t_def, const VertexLayout& t_layout, PrimitiveTopology t_topology);
		Shader* GetOrCreateShaderInternal(const std::string& path, ShaderStage stage);
	};
}
