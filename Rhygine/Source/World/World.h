#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <optional>
#include <atomic>

#include "ArchetypeSignature.h"
#include "Archetype.h"
#include "Behaviour.h"
#include "WorldDescriptors.h"
#include "DeferredCommandBuffer.h"
#include "TypeRegistry.h"
#include "ResourceManager.h"
#include "Service.h"

namespace Rhygine
{
	class Scene;
	class RenderPacketBuilder;
	class SceneRenderer;
	class IDevice;

	class World
	{
		friend class DeferredCommandBuffer;

	public:
		World(SceneRenderer& t_sceneRenderer, IDevice& t_device);

		void Tick(DeltaTime t_deltaTime);

		// =============================================================================
		// Entity Lifecycle
		// =============================================================================

		[[nodiscard]] EntityId CreateEntity(SceneId t_sceneId);
		void DestroyEntity(EntityId t_entityId);
		[[nodiscard]] bool IsAlive(EntityId t_entityId) const;

		// =============================================================================
		// Component Access and Manipulation
		// =============================================================================

		// TODO: The GetComponent and HasComponent don't factor in parent-child relationships.
		// Also maybe all GetAllComponnents as well.

		template <typename T>
		T* AddComponent(EntityId t_entityId, const void* t_data)
		{
			TypeId id = m_TypeRegistry.GetComponentTypeId<T>();
			void* component = AddComponent(id, t_entityId, t_data);
			return static_cast<T*>(component);
		}
		void* AddComponent(EntityId t_entityId, TypeId t_type, const void* t_data);

		template <typename T>
		T* GetComponent(EntityId t_entityId) const
		{
			TypeId id = m_TypeRegistry.GetComponentTypeId<T>();
			void* component = GetComponent(t_entityId, id);
			return static_cast<T*>(component);
		}
		void* GetComponent(EntityId t_entityId, TypeId t_type) const;

		template <typename T>
		bool HasComponent(EntityId t_entityId) const
		{
			TypeId id = m_TypeRegistry.GetComponentTypeId<T>();
			return HasComponent(t_entityId, id);
		}
		bool HasComponent(EntityId t_entityId, TypeId t_type) const;

		template <typename T>
		bool RemoveComponent(EntityId t_entityId)
		{
			TypeId id = m_TypeRegistry.GetComponentTypeId<T>();
			return RemoveComponent(t_entityId, id);
		}
		bool RemoveComponent(EntityId t_entityId, TypeId t_type);

		// =============================================================================
		// Behaviour Access and Manipulation
		// =============================================================================

		template <typename T, typename... Args>
		void AddBehaviour(EntityId t_entityId, Args&&... t_args)
		{
			static TypeId id = m_TypeRegistry.GetBehaviourTypeId<T>();
			if constexpr (sizeof...(Args) == 0)
			{
				AddBehaviour(t_entityId, id, nullptr);
			}
			else if constexpr (sizeof...(Args) == 1)
			{
				([&](auto&& arg) {
					AddBehaviour(t_entityId, id, const_cast<void*>(static_cast<const void*>(&arg)));
					}(std::forward<Args>(t_args)), ...);
			}
			else
			{
				auto argsTuple = std::make_tuple(std::forward<Args>(t_args)...);
				AddBehaviour(t_entityId, id, &argsTuple);
			}
		}
		void AddBehaviour(EntityId t_entityId, TypeId t_type, void* t_args);

		template <typename T>
		T* GetBehaviour(EntityId t_entityId) const
		{
			static TypeId id = m_TypeRegistry.GetBehaviourTypeId<T>();
			return static_cast<T*>(GetBehaviour(t_entityId, id));
		}
		Behaviour* GetBehaviour(EntityId t_entityId, TypeId t_type) const;

		template <typename T>
		bool HasBehaviour(EntityId t_entityId) const
		{
			static TypeId id = m_TypeRegistry.GetBehaviourTypeId<T>();
			return HasBehaviour(t_entityId, id);
		}
		bool HasBehaviour(EntityId t_entityId, TypeId t_type) const;

		template <typename T>
		bool RemoveBehaviour(EntityId t_entityId)
		{
			static TypeId id = m_TypeRegistry.GetBehaviourTypeId<T>();
			return RemoveBehaviour(t_entityId, id);
		}
		bool RemoveBehaviour(EntityId t_entityId, TypeId t_type);

		template <typename... Ts>
		void ForEach(std::function<void(EntityId, Ts&...)> t_fn)
		{
			m_inForEach.store(true);

			for (Archetype* archetype : GetMatchingArchetypes<Ts...>())
			{
				for (uint32_t row = 0; row < archetype->count; ++row)
				{
					EntityId id = archetype->entities[row];
					t_fn(id, *GetColumn<Ts>(archetype, row)...);
				}
			}

			m_inForEach.store(false);

			m_deferredCommandBuffer.Flush(*this);
		}

		const TypeRegistry& GetTypeRegistry() const { return m_TypeRegistry; }

		void BuildRenderPacket(RenderPacketBuilder& t_builder) const;

		// =============================================================================
		// Resource Management
		// =============================================================================

		template <typename T>
		ResourceHandle<T> LoadResource(const std::string& t_path)
		{
			return m_resourceManager.Load<T>(t_path);
		}

		template <typename T>
		ResourceHandle<T> GetResource(const std::string& t_path)
		{
			return m_resourceManager.Get<T>(t_path);
		}

		ResourceManager& GetResourceManager();

		// =============================================================================
		// Services
		// =============================================================================

		template <typename T, typename... Args>
		T* AddService(Args&&... t_args)
		{
			static TypeId id = m_TypeRegistry.GetServiceTypeId<T>();
			if (HasService(id))
			{
				return nullptr;
			}
			if constexpr (sizeof...(Args) == 0)
			{
				return AddService(id, nullptr);
			}
			else if constexpr (sizeof...(Args) == 1)
			{
				([&](auto&& arg) {
					return AddService(id, const_cast<void*>(static_cast<const void*>(&arg)));
					}(std::forward<Args>(t_args)), ...);
			}
			else
			{
				auto argsTuple = std::make_tuple(std::forward<Args>(t_args)...);
				return AddService(id, &argsTuple);
			}
		}
		Service* AddService(TypeId t_type, void* t_args);

		template <typename T>
		T* GetService()
		{
			static TypeId id = m_TypeRegistry.GetServiceTypeId<T>();
			return static_cast<T*>(GetService(id));
		}
		Service* GetService(TypeId t_type);

		template <typename T>
		bool HasService() const
		{
			static TypeId id = m_TypeRegistry.GetServiceTypeId<T>();
			return HasService(id);
		}
		bool HasService(TypeId t_type) const;

		template <typename T>
		bool RemoveService()
		{
			static TypeId id = m_TypeRegistry.GetServiceTypeId<T>();
			return RemoveService(id);
		}
		bool RemoveService(TypeId t_type);

	private:
		std::vector<EntityRecord> m_entityRecords;
		std::queue<uint32_t> m_freeEntityIndices;

		std::unordered_map<ArchetypeSignature, std::unique_ptr<Archetype>> m_archetypes;
		std::vector<Archetype*> m_extractorArchetypes;
		std::unordered_map<EntityId, std::vector<std::unique_ptr<Behaviour>>> m_behaviours;

		TypeRegistry m_TypeRegistry;
		DeferredCommandBuffer m_deferredCommandBuffer;

		ResourceManager m_resourceManager;

		std::vector<std::unique_ptr<Service>> m_services;
		std::vector<Service*> m_toRemoveServices;

		SceneRenderer& m_sceneRenderer;
		IDevice& m_device;

		std::atomic_bool m_inParallelPhase = false;
		std::atomic_bool m_inForEach = false;

		inline bool ShouldDeferCommands() const { return m_inParallelPhase.load() || m_inForEach.load(); }

		void* InnerAddComponent(EntityId t_entityId, TypeId t_type, const void* t_data);
		void* InnerGetComponent(EntityId t_entityId, TypeId t_type) const;
		bool InnerHasComponent(EntityId t_entityId, TypeId t_type) const;
		bool InnerRemoveComponent(EntityId t_entityId, TypeId t_type);

		Behaviour* InnerAddBehaviour(EntityId t_entityId, TypeId t_type, void* t_args);
		Behaviour* InnerGetBehaviour(EntityId t_entityId, TypeId t_type) const;
		bool InnerHasBehaviour(EntityId t_entityId, TypeId t_type) const;
		bool InnerRemoveBehaviour(EntityId t_entityId, TypeId t_type);

		void InnerAddTypeDependencies(EntityId t_entityId, TypeId t_type);
		void InnerRemoveTypeDependents(EntityId t_entityId, TypeId t_type);

		void CreateEntityImmediate(EntityId t_id);
		void* AddComponentImmediate(EntityId t_id, TypeId t_type, const void* t_data);
		Behaviour* AddBehaviourImmediate(EntityId t_id, TypeId t_type, void* t_args);
		void DestroyEntityImmediate(EntityId t_id);
		void RemoveComponentImmediate(EntityId t_id, TypeId t_type);
		void RemoveBehaviourImmediate(EntityId t_id, TypeId t_type);

		Archetype& GetOrCreateArchetype(const ArchetypeSignature& t_signature);
	};
}
