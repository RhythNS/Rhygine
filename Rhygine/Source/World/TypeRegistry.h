#pragma once

#include <atomic>
#include <unordered_map>
#include <vector>
#include <string>
#include <optional>
#include <new>
#include <utility>
#include <tracy/Tracy.hpp>

#include "WorldDescriptors.h"
#include "Debug/Error.h"

namespace Rhygine
{
	namespace ComponentLifecycle
	{
		template <typename T>
		void ConstructDefault(void* ptr)
		{
			new (ptr) T();
		}

		template <typename T>
		void Destruct(void* ptr)
		{
			static_cast<T*>(ptr)->~T();
		}

		template <typename T>
		void MoveConstruct(void* dst, void* src)
		{
			new (dst) T(std::move(*static_cast<T*>(src)));
		}

		template <typename T>
		std::unique_ptr<void, void(*)(void*)> Factory(void* t_args)
		{
			return std::unique_ptr<void, void(*)(void*)>(new T(*static_cast<T*>(t_args)), [](void* ptr) { delete static_cast<T*>(ptr); });
		}
	}

	class TypeRegistry
	{
	public:
		template <typename T>
		TypeId RegisterComponent(TypeInfo::ExtractorFn t_extractor = nullptr, TypeInfo::AttachFn t_attach = nullptr, TypeInfo::DetachFn t_detach = nullptr)
		{
			static const TypeId id = Register(TypeInfo
				{
				.name = typeid(T).name(),
				.kind = TypeKind::Component,
				.size = sizeof(T),
				.alignment = alignof(T),
				.construct = &ComponentLifecycle::ConstructDefault<T>,
				.destruct = &ComponentLifecycle::Destruct<T>,
				.moveConstruct = &ComponentLifecycle::MoveConstruct<T>,
				.attach = t_attach,
				.detach = t_detach,
				.extractor = t_extractor
				}
			);
			return id;
		}

		template <typename T>
		TypeId RegisterBehaviour()
		{
			static const TypeId id = Register(TypeInfo
				{
				.name = typeid(T).name(),
				.kind = TypeKind::Behaviour,
				.factory = &ComponentLifecycle::Factory<T>
				});
			return id;
		}

		template <typename T>
		TypeId RegisterService()
		{
			static const TypeId id = Register(TypeInfo
				{
				.name = typeid(T).name(),
				.kind = TypeKind::Service,
				.factory = &ComponentLifecycle::Factory<T>
				});
			return id;
		}

		template <typename T>
		TypeId RegisterTask()
		{
			static const TypeId id = Register(TypeInfo
				{
				.name = typeid(T).name(),
				.kind = TypeKind::Task,
				.factory = &ComponentLifecycle::Factory<T>
				});
			return id;
		}

		TypeId Register(const TypeInfo& t_info);

		template <typename T>
		TypeId GetComponentTypeId() const
		{
			ZoneScoped;

			static const TypeId id = FindByName(typeid(T).name()).value_or(UINT32_MAX);
			ASSERT_ERROR_MESSAGE(id != UINT32_MAX, "Component type not registered: " + std::string(typeid(T).name()));
			return id;
		}
		template <typename T>
		TypeId GetBehaviourTypeId() const
		{
			ZoneScoped;

			static const TypeId id = FindByName(typeid(T).name()).value_or(UINT32_MAX);
			ASSERT_ERROR_MESSAGE(id != UINT32_MAX, "Behaviour type not registered: " + std::string(typeid(T).name()));
			return id;
		}
		template <typename T>
		TypeId GetServiceTypeId() const
		{
			ZoneScoped;

			static const TypeId id = FindByName(typeid(T).name()).value_or(UINT32_MAX);
			ASSERT_ERROR_MESSAGE(id != UINT32_MAX, "Service type not registered: " + std::string(typeid(T).name()));
			return id;
		}

		template <typename T>
		TypeId GetTaskTypeId() const
		{
			ZoneScoped;

			static const TypeId id = FindByName(typeid(T).name()).value_or(UINT32_MAX);
			ASSERT_ERROR_MESSAGE(id != UINT32_MAX, "Task type not registered: " + std::string(typeid(T).name()));
			return id;
		}

		template <typename T>
		TypeId GetId() const
		{
			auto opt = FindByName(typeid(T).name());
			return opt.value_or(UINT32_MAX);
		}

		std::optional<TypeId> FindByName(const std::string& t_name) const;
		const TypeInfo& GetTypeInfo(TypeId t_id) const;
		TypeKind GetKind(TypeId t_id) const;

		void RegisterDependency(TypeId t_dependent, TypeId t_required);
		void RegisterParentChild(TypeId t_parent, TypeId t_child);
		const std::vector<TypeId>& GetDependencies(TypeId t_id) const;
		const std::vector<TypeId>& GetDependents(TypeId t_id) const;
		const std::vector<TypeId>& GetChildren(TypeId t_id) const;
		const std::optional<TypeId> GetParent(TypeId t_id) const;

	private:
		std::vector<TypeInfo> m_types;
		std::unordered_map<std::string, TypeId> m_nameToId;
		std::atomic<TypeId> m_nextId{ 0 };

		std::unordered_map<TypeId, std::vector<TypeId>> m_dependencies;
		std::unordered_map<TypeId, std::vector<TypeId>> m_dependents;
		std::unordered_map<TypeId, std::vector<TypeId>> m_children;
		std::unordered_map<TypeId, TypeId> m_parents;
	};
}
