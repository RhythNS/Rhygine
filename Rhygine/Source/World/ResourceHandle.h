#pragma once

#include "ResourceEntry.h"

namespace Rhygine
{
	template <typename T>
	class ResourceHandle
	{
		friend class ResourceManager;

	public:
		ResourceHandle() = default;
		T* Get() const
		{
			if (!m_entry || !IsLoaded())
			{
				return nullptr;
			}
			return m_entry->Get();
		}
		bool IsLoaded() const { return m_entry ? m_entry->IsLoaded() : false; }

		Resource::State GetState() const
		{
			return m_entry ? m_entry->GetState() : Resource::State::Unloaded;
		}

		const std::string& GetPath() const
		{
			static const std::string empty;
			return m_entry ? m_entry->GetPath() : empty;
		}

		bool IsValid() const { return m_entry != nullptr; }

		explicit operator bool() const { return IsValid() && IsLoaded(); }
		T* operator->() const { return Get(); }
		T& operator*() const { return *Get(); }

	private:
		explicit ResourceHandle(std::shared_ptr<ResourceEntry<T>> t_entry)
			: m_entry(std::move(t_entry)) {}

		std::shared_ptr<ResourceEntry<T>> m_entry = nullptr;
	};
}
