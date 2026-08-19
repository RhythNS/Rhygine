#pragma once

#include <string>
#include <memory>
#include <type_traits>

#include "Resource.h"
#include "WorldDescriptors.h"

namespace Rhygine
{
	class ResourceManager;
	template <typename T> class ResourceHandle;

	class ResourceEntryBase
	{
	public:
		virtual ~ResourceEntryBase() = default;
		ResourceEntryBase(std::string t_path, TypeId t_typeId)
			: m_path(std::move(t_path))
		{}

		const std::string& GetPath() const { return m_path; }

		virtual Resource* GetRawResource() = 0;
		virtual const Resource* GetRawResource() const = 0;

		virtual Resource::State GetState() const = 0;
		virtual bool IsLoaded() const = 0;
		virtual void Unload() = 0;

	protected:
		std::string m_path;
	};

	template <typename T>
	class ResourceEntry : public ResourceEntryBase
	{
		static_assert(std::is_base_of_v<Resource, T>, "T must derive from Rhygine::Resource");

	public:
		ResourceEntry(std::string t_path, TypeId t_typeId)
			: ResourceEntryBase(std::move(t_path), t_typeId)
			, m_resource(std::make_unique<T>())
		{}

		T* Get() { return m_resource.get(); }
		const T* Get() const { return m_resource.get(); }

		Resource* GetRawResource() override { return m_resource.get(); }
		const Resource* GetRawResource() const override { return m_resource.get(); }

		Resource::State GetState() const override
		{
			return m_resource ? m_resource->GetState() : Resource::State::Unloaded;
		}
		bool IsLoaded() const override
		{
			if (!m_resource)
			{
				return false;
			}
			const auto state = m_resource->GetState();
			bool needsGPUUpload = m_resource->NeedsGPUUpload();

			return state != Resource::State::Failed && 
				((!needsGPUUpload && state >= Resource::State::CPUReady) ||
				(needsGPUUpload && state >= Resource::State::GPUReady));
		}

	private:
		std::unique_ptr<T> m_resource;
	};
}
