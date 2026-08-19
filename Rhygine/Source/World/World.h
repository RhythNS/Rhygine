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

namespace Rhygine
{
	class Scene;
	class RenderPacketBuilder;

	class World
	{
		friend class DeferredCommandBuffer;

	public:
		World();

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
		T* AddComponent(EntityId t_entityId, const void* t_data);
		void* AddComponent(EntityId t_entityId, TypeId t_type, const void* t_data);

		template <typename T>
		T* GetComponent(EntityId t_entityId) const;
		void* GetComponent(EntityId t_entityId, TypeId t_type) const;

		template <typename T>
		bool HasComponent(EntityId t_entityId) const;
		bool HasComponent(EntityId t_entityId, TypeId t_type) const;

		template <typename T>
		bool RemoveComponent(EntityId t_entityId);
		bool RemoveComponent(EntityId t_entityId, TypeId t_type);

		// =============================================================================
		// Behaviour Access and Manipulation
		// =============================================================================

		template <typename T, typename... Args>
		void AddBehaviour(EntityId t_entityId, Args&&... t_args);
		void AddBehaviour(EntityId t_entityId, TypeId t_type, void* t_args);

		template <typename T>
		T* GetBehaviour(EntityId t_entityId) const;
		void* GetBehaviour(EntityId t_entityId, TypeId t_type) const;

		template <typename T>
		bool HasBehaviour(EntityId t_entityId) const;
		bool HasBehaviour(EntityId t_entityId, TypeId t_type) const;

		template <typename T>
		bool RemoveBehaviour(EntityId t_entityId);
		bool RemoveBehaviour(EntityId t_entityId, TypeId t_type);

		template <typename... Ts>
		void ForEach(std::function<void(EntityId, Ts&...)> t_fn);

		const TypeRegistry& GetTypeRegistry() const { return m_TypeRegistry; }

		void BuildRenderPacket(RenderPacketBuilder& t_builder) const;
		
	private:
		std::vector<EntityRecord> m_entityRecords;
		std::queue<uint32_t> m_freeEntityIndices;

		std::unordered_map<ArchetypeSignature, std::unique_ptr<Archetype>> m_archetypes;
		std::vector<Archetype*> m_extractorArchetypes;
		std::unordered_map<uint32_t, std::vector<std::unique_ptr<Behaviour>>> m_behaviours;

		TypeRegistry m_TypeRegistry;
		DeferredCommandBuffer m_deferredCommandBuffer;

		std::atomic_bool m_inParallelPhase = false;
		std::atomic_bool m_inForEach = false;

		inline bool ShouldDeferCommands() const { return m_inParallelPhase.load() || m_inForEach.load(); }

		void* InnerAddComponent(EntityId t_entityId, TypeId t_type, const void* t_data);
		void* InnerGetComponent(EntityId t_entityId, TypeId t_type) const;
		bool InnerHasComponent(EntityId t_entityId, TypeId t_type) const;
		bool InnerRemoveComponent(EntityId t_entityId, TypeId t_type);

		void* InnerAddBehaviour(EntityId t_entityId, TypeId t_type, void* t_args);
		void* InnerGetBehaviour(EntityId t_entityId, TypeId t_type) const;
		bool InnerHasBehaviour(EntityId t_entityId, TypeId t_type) const;
		bool InnerRemoveBehaviour(EntityId t_entityId, TypeId t_type);

		void InnerAddTypeDependencies(EntityId t_entityId, TypeId t_type);
		void InnerRemoveTypeDependents(EntityId t_entityId, TypeId t_type);

		void CreateEntityImmediate(EntityId t_id);
		void* AddComponentImmediate(EntityId t_id, TypeId t_type, const void* t_data);
		void* AddBehaviourImmediate(EntityId t_id, TypeId t_type, void* t_args);
		void DestroyEntityImmediate(EntityId t_id);
		void RemoveComponentImmediate(EntityId t_id, TypeId t_type);
		void RemoveBehaviourImmediate(EntityId t_id, TypeId t_type);

		Archetype& GetOrCreateArchetype(const ArchetypeSignature& t_signature);
	};
}
