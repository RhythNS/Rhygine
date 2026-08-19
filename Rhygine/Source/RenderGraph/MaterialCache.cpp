#include "MaterialCache.h"

#include <unordered_set>
#include <format>
#include <tracy/Tracy.hpp>

#include "System.h"
#include "File/FileManager.h"
#include "StructuredIO/Read.h"
#include "Debug/Logger.h"

namespace Rhygine
{
	MaterialCache::MaterialCache(IDevice* t_device, FileManager* t_fileManager)
		: m_device(t_device)
		, m_fileManager(t_fileManager)
	{}

	MaterialCache::~MaterialCache()
	{
		InvalidateAll();
	}

	Pipeline* MaterialCache::GetOrCreatePipeline(
		const MaterialDefinition* t_def,
		const VertexLayout& t_layout,
		PrimitiveTopology t_topology)
	{
		ZoneScoped;
		std::lock_guard<Mutex> lock(m_mutex);
		return GetOrCreatePipelineInternal(t_def, t_layout, t_topology);
	}

	Pipeline* MaterialCache::GetOrCreatePipelineInternal(
		const MaterialDefinition* t_def,
		const VertexLayout& t_layout,
		PrimitiveTopology t_topology)
	{
		ZoneScoped;
		if (!t_def || !m_device)
		{
			return nullptr;
		}

		PipelineKey key{ t_def, t_layout.hash, t_topology };
		auto it = m_pipelineCache.find(key);
		if (it != m_pipelineCache.end())
		{
			return it->second;
		}

		LOG_DEBUG(std::format("MaterialCache: Creating new pipeline for '{}'", t_def->GetVertexShaderPath()));

		Shader* vertexShader = GetOrCreateShaderInternal(t_def->GetVertexShaderPath(), ShaderStage::Vertex);
		if (!vertexShader)
		{
			return nullptr;
		}

		Shader* pixelShader = nullptr;
		if (!t_def->GetPixelShaderPath().empty())
		{
			pixelShader = GetOrCreateShaderInternal(t_def->GetPixelShaderPath(), ShaderStage::Pixel);
			if (!pixelShader)
			{
				return nullptr;
			}
		}

		GraphicsPipelineDesc pipeDesc{};
		pipeDesc.vertexShader = vertexShader;
		pipeDesc.pixelShader = pixelShader;
		pipeDesc.blendState = t_def->GetBlendState();
		pipeDesc.rasterizerState = t_def->GetRasterizerState();
		pipeDesc.depthStencilState = t_def->GetDepthStencilState();
		pipeDesc.topology = t_topology;
		pipeDesc.vertexBindings = t_layout.bindings;
		pipeDesc.vertexAttributes = t_layout.attributes;
		pipeDesc.renderTargetFormats = { Format::B8G8R8A8_UNORM };
		pipeDesc.depthStencilFormat = Format::D32_SFLOAT;
		pipeDesc.debugName = t_def->GetVertexShaderPath();

		Pipeline* pipeline = m_device->CreateGraphicsPipeline(pipeDesc);
		if (pipeline)
		{
			m_pipelineCache[key] = pipeline;
		}
		return pipeline;
	}

	Material* MaterialCache::GetOrCreate(const MaterialDefinition* t_def)
	{
		ZoneScoped;
		std::lock_guard<Mutex> lock(m_mutex);

		if (!t_def || !m_device)
		{
			return nullptr;
		}

		auto it = m_cache.find(t_def);
		if (it != m_cache.end())
		{
			return it->second.get();
		}

		Pipeline* pipeline = GetOrCreatePipelineInternal(t_def, VertexLayout::PositionColor(), PrimitiveTopology::TriangleList);
		if (!pipeline)
		{
			return nullptr;
		}

		ResourceLayout* layout = nullptr;
		ResourceSet* resourceSet = nullptr;

		uint32_t materialId = m_nextMaterialId++;
		auto material = std::make_unique<Material>(materialId, pipeline, layout, resourceSet, t_def);
		Material* ptr = material.get();
		m_cache[t_def] = std::move(material);
		return ptr;
	}

	void MaterialCache::Invalidate(const MaterialDefinition* t_def)
	{
		ZoneScoped;
		LOG_DEBUG("MaterialCache: Invalidating material definition");
		std::lock_guard<Mutex> lock(m_mutex);
		auto it = m_cache.find(t_def);
		if (it != m_cache.end())
		{
			m_cache.erase(it);
		}

		if (m_device)
		{
			for (auto pit = m_pipelineCache.begin(); pit != m_pipelineCache.end(); )
			{
				if (pit->first.definition == t_def)
				{
					if (pit->second)
					{
						m_device->DestroyPipeline(pit->second);
					}
					pit = m_pipelineCache.erase(pit);
				}
				else
				{
					++pit;
				}
			}
		}
	}

	void MaterialCache::QueueInvalidation(const MaterialDefinition* t_def)
	{
		ZoneScoped;
		std::lock_guard<Mutex> lock(m_mutex);
		m_pendingInvalidations.push_back(t_def);
	}

	void MaterialCache::ProcessPendingInvalidations()
	{
		ZoneScoped;
		LOG_DEBUG("MaterialCache: Processing pending invalidations");
		std::lock_guard<Mutex> lock(m_mutex);
		for (const auto* def : m_pendingInvalidations)
		{
			auto it = m_cache.find(def);
			if (it != m_cache.end())
			{
				m_cache.erase(it);
			}

			if (m_device)
			{
				for (auto pit = m_pipelineCache.begin(); pit != m_pipelineCache.end(); )
				{
					if (pit->first.definition == def)
					{
						if (pit->second)
						{
							m_device->DestroyPipeline(pit->second);
						}
						pit = m_pipelineCache.erase(pit);
					}
					else
					{
						++pit;
					}
				}
			}
		}
		m_pendingInvalidations.clear();
	}

	void MaterialCache::InvalidateAll()
	{
		ZoneScoped;
		LOG_INFO("MaterialCache: Invalidating all cached materials, pipelines, and shaders");
		std::lock_guard<Mutex> lock(m_mutex);

		if (m_device)
		{
			std::unordered_set<Pipeline*> destroyedPipelines;

			for (auto& [key, pipeline] : m_pipelineCache)
			{
				if (pipeline && !destroyedPipelines.contains(pipeline))
				{
					m_device->DestroyPipeline(pipeline);
					destroyedPipelines.insert(pipeline);
				}
			}

			for (auto& [def, mat] : m_cache)
			{
				if (mat && mat->GetPipeline() && !destroyedPipelines.contains(mat->GetPipeline()))
				{
					m_device->DestroyPipeline(mat->GetPipeline());
					destroyedPipelines.insert(mat->GetPipeline());
				}
			}

			for (auto& [path, shader] : m_shaderCache)
			{
				if (shader)
				{
					m_device->DestroyShader(shader);
				}
			}
		}

		m_cache.clear();
		m_pipelineCache.clear();
		m_shaderCache.clear();
		m_pendingInvalidations.clear();
	}

	Shader* MaterialCache::GetOrCreateShader(const std::string& path, ShaderStage stage)
	{
		ZoneScoped;
		std::lock_guard<Mutex> lock(m_mutex);
		return GetOrCreateShaderInternal(path, stage);
	}

	Shader* MaterialCache::GetOrCreateShaderInternal(const std::string& path, ShaderStage stage)
	{
		ZoneScoped;
		if (path.empty() || !m_device)
		{
			return nullptr;
		}

		auto it = m_shaderCache.find(path);
		if (it != m_shaderCache.end())
		{
			return it->second;
		}

		LOG_DEBUG(std::format("MaterialCache: Loading shader '{}'", path));
		FileManager* fileManager = m_fileManager;
		if (!fileManager)
		{
			System* system = System::GetInstance();
			if (system)
			{
				fileManager = &system->GetFileManager();
			}
		}

		if (!fileManager)
		{
			LOG_ERROR("MaterialCache: No FileManager available to load shader");
			return nullptr;
		}

		auto file = fileManager->Open(path, static_cast<FileMode>(FileMode::Read | FileMode::Binary));
		if (!file || !file->Good())
		{
			LOG_WARN(std::format("MaterialCache: Could not open shader file '{}'", path));
			return nullptr;
		}

		std::vector<char> bytecode = file->ReadAll();
		if (bytecode.empty())
		{
			LOG_WARN(std::format("MaterialCache: Shader file '{}' is empty", path));
			return nullptr;
		}

		ShaderDesc desc{};
		desc.stage = stage;
		desc.bytecode = bytecode.data();
		desc.bytecodeSize = bytecode.size();
		desc.entryPointName = "main";
		desc.debugName = path;

		Shader* shader = m_device->CreateShader(desc);
		if (shader)
		{
			m_shaderCache[path] = shader;
		}
		return shader;
	}
}
