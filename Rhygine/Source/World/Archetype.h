#pragma once

#include <vector>
#include <unordered_map>
#include <optional>

#include "WorldDescriptors.h"
#include "ArchetypeSignature.h"

namespace Rhygine
{
	class TypeRegistry;
	class RenderPacketBuilder;

	class Archetype
	{
	public:
		struct ComponentColumn
		{
			std::vector<std::byte> data;
			size_t elementSize = 0;
			size_t alignment = 0;

			TypeInfo::ConstructFn constructor = nullptr;
			TypeInfo::MoveConstructFn moveConstruct = nullptr;
			TypeInfo::DestructFn destructor = nullptr;

			void* GetRow(size_t t_row) { return data.data() + (t_row * elementSize); }
			const void* GetRow(size_t t_row) const { return data.data() + (t_row * elementSize); }
		};

		struct MovedEntityInfo
		{
			EntityId entity;
			size_t newRow;
		};

		struct MigrationResult
		{
			size_t targetRow;
			std::optional<MovedEntityInfo> sourceMovedEntity;
		};

		struct ExtractorInfo
		{
			TypeId componentType;
			TypeInfo::ExtractorFn extractor;
		};

		Archetype() = delete;
		Archetype(const ArchetypeSignature& t_signature, const TypeRegistry& t_registry);

		const ArchetypeSignature& GetSignature() const { return m_signature; }
		[[nodiscard]] size_t GetEntityCount() const { return m_entities.size(); }
		[[nodiscard]] const std::vector<EntityId>& GetEntities() const { return m_entities; }

		[[nodiscard]] bool NeedsExtraction() const { return !m_extractors.empty(); }
		[[nodiscard]] const std::vector<ExtractorInfo>& GetExtractors() const { return m_extractors; }
		[[nodiscard]] void BuildRenderPacket(RenderPacketBuilder& t_builder, const TypeRegistry& t_registry);

		[[nodiscard]] bool ContainsComponent(TypeId t_componentType) const
		{
			return m_signature.Contains(t_componentType);
		}
		[[nodiscard]] void* GetComponentData(TypeId t_componentType, size_t index)
		{
			auto it = m_componentColumns.find(t_componentType);
			if (it != m_componentColumns.end())
			{
				return it->second.GetRow(index);
			}
			return nullptr;
		}
		[[nodiscard]] const void* GetComponentData(TypeId t_componentType, size_t index) const
		{
			return GetComponentData(t_componentType, index);
		}

		[[nodiscard]] ComponentColumn* GetColumn(TypeId t_componentType)
		{
			auto it = m_componentColumns.find(t_componentType);
			return (it != m_componentColumns.end()) ? &it->second : nullptr;
		}
		[[nodiscard]] const ComponentColumn* GetColumn(TypeId t_componentType) const
		{
			return GetColumn(t_componentType);
		}

		[[nodiscard]] size_t AllocateEntity(EntityId t_entityId);
		[[nodiscard]] std::optional<MovedEntityInfo> RemoveEntityByRow(size_t t_row);
		[[nodiscard]] MigrationResult MoveEntityTo(
			size_t t_sourceRow,
			Archetype& t_targetArchetype,
			TypeId t_newComponentType = 0,
			void* t_newComponentData = nullptr
		);

		[[nodiscard]] Archetype* GetAddEdge(TypeId t_type) const;
		void SetAddEdge(TypeId t_type, Archetype* t_target);
		[[nodiscard]] Archetype* GetRemoveEdge(TypeId t_type) const;
		void SetRemoveEdge(TypeId t_type, Archetype* t_target);

	private:
		ArchetypeSignature m_signature;
		std::unordered_map<TypeId, ComponentColumn> m_componentColumns;
		std::vector<EntityId> m_entities;
		std::vector<ExtractorInfo> m_extractors;

		std::unordered_map<TypeId, Archetype*> m_addEdges;
		std::unordered_map<TypeId, Archetype*> m_removeEdges;
	};
}
