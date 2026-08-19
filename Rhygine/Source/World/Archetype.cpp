#include "Archetype.h"

#include "TypeRegistry.h"
#include "Components/Transform.h"
#include "Debug/Error.h"

Rhygine::Archetype::Archetype(const ArchetypeSignature& t_signature, const TypeRegistry& t_registry)
	: m_signature(t_signature)
{
	for (TypeId typeId : m_signature.GetComponentTypes())
	{
		const TypeInfo& info = t_registry.GetTypeInfo(typeId);
		ComponentColumn column;
		column.elementSize = info.size;
		column.alignment = info.alignment;
		column.constructor = info.construct;
		column.destructor = info.destruct;
		column.moveConstruct = info.moveConstruct;

		m_componentColumns.emplace(typeId, std::move(column));

		if (info.extractor)
		{
			m_extractors.push_back({ typeId, info.extractor });
		}
	}
}

void Rhygine::Archetype::BuildRenderPacket(RenderPacketBuilder& t_packet, const TypeRegistry& t_registry)
{
	TypeId transformTypeId = t_registry.GetComponentTypeId<Transform>();
	ComponentColumn* transformColumn = GetColumn(transformTypeId);
	const std::vector<Archetype::ExtractorInfo>& extractors = GetExtractors();

	for (auto& extractorInfo : extractors)
	{
		for (uint32_t row = 0; row < GetEntityCount(); ++row)
		{
			Transform* transformPtr = nullptr;
			if (transformColumn)
			{
				transformPtr = reinterpret_cast<Transform*>(transformColumn->GetRow(row));
			}
			void* componentPtr = GetComponentData(extractorInfo.componentType, row);
			extractorInfo.extractor(componentPtr, transformPtr, t_packet);
		}
	}
}

size_t Rhygine::Archetype::AllocateEntity(EntityId t_entityId)
{
	size_t index = m_entities.size();
	m_entities.push_back(t_entityId);

	for (auto& [typeId, column] : m_componentColumns)
	{
		column.data.resize((index + 1) * column.elementSize);
		void* rowPtr = column.GetRow(index);
		if (column.constructor)
		{
			column.constructor(rowPtr);
		}
	}
	return index;
}

std::optional<Rhygine::Archetype::MovedEntityInfo> Rhygine::Archetype::RemoveEntityByRow(size_t t_row)
{
	if (t_row >= m_entities.size())
	{
		STOP_EXECUTION_MESSAGE("Row out of range in Archetype::RemoveEntityByRow");
		return std::nullopt;
	}

	size_t lastIndex = m_entities.size() - 1;

	if (t_row == lastIndex)
	{
		for (auto& [typeId, column] : m_componentColumns)
		{
			void* rowPtr = column.GetRow(t_row);
			if (column.destructor)
			{
				column.destructor(rowPtr);
			}
			column.data.resize(lastIndex * column.elementSize);
		}
		m_entities.pop_back();

		return std::nullopt;
	}

	EntityId lastEntity = m_entities.back();
	for (auto& [typeId, column] : m_componentColumns)
	{
		void* targetRowPtr = column.GetRow(t_row);
		void* lastRowPtr = column.GetRow(lastIndex);

		if (column.destructor)
		{
			column.destructor(targetRowPtr);
		}
		if (column.moveConstruct)
		{
			column.moveConstruct(targetRowPtr, lastRowPtr);
		}
		if (column.destructor)
		{
			column.destructor(lastRowPtr);
		}
		column.data.resize(lastIndex * column.elementSize);
	}

	m_entities[t_row] = lastEntity;
	m_entities.pop_back();

	return MovedEntityInfo{ lastEntity, t_row };
}

Rhygine::Archetype::MigrationResult Rhygine::Archetype::MoveEntityTo(size_t t_sourceRow, Archetype& t_targetArchetype, TypeId t_newComponentType, void* t_newComponentData)
{
	if (t_sourceRow >= m_entities.size())
	{
		STOP_EXECUTION_MESSAGE("Source row out of range in Archetype::MoveEntityTo");
		return { 0, std::nullopt };
	}

	EntityId entityId = m_entities[t_sourceRow];
	size_t targetRow = t_targetArchetype.m_entities.size();
	t_targetArchetype.m_entities.push_back(entityId);

	for (auto& [typeId, targetColumn] : t_targetArchetype.m_componentColumns)
	{
		targetColumn.data.resize((targetRow + 1) * targetColumn.elementSize);
		void* dstPtr = targetColumn.GetRow(targetRow);

		auto srcIt = m_componentColumns.find(typeId);
		if (srcIt != m_componentColumns.end())
		{
			void* srcPtr = srcIt->second.GetRow(t_sourceRow);
			if (targetColumn.moveConstruct)
			{
				targetColumn.moveConstruct(dstPtr, srcPtr);
			}
		}
		else if (typeId == t_newComponentType && t_newComponentData != nullptr)
		{
			if (targetColumn.moveConstruct)
			{
				targetColumn.moveConstruct(dstPtr, t_newComponentData);
			}
		}
		else
		{
			if (targetColumn.constructor)
			{
				targetColumn.constructor(dstPtr);
			}
		}
	}

	for (auto& [typeId, srcColumn] : m_componentColumns)
	{
		if (!t_targetArchetype.ContainsComponent(typeId))
		{
			void* srcPtr = srcColumn.GetRow(t_sourceRow);
			if (srcColumn.destructor)
			{
				srcColumn.destructor(srcPtr);
			}
		}
	}

	std::optional<MovedEntityInfo> sourceMoved = RemoveEntityByRow(t_sourceRow);

	return MigrationResult{ targetRow, sourceMoved };
}

Rhygine::Archetype* Rhygine::Archetype::GetAddEdge(TypeId t_type) const
{
	auto it = m_addEdges.find(t_type);
	return (it != m_addEdges.end()) ? it->second : nullptr;
}

void Rhygine::Archetype::SetAddEdge(TypeId t_type, Archetype* t_target)
{
	m_addEdges[t_type] = t_target;
}

Rhygine::Archetype* Rhygine::Archetype::GetRemoveEdge(TypeId t_type) const
{
	auto it = m_removeEdges.find(t_type);
	return (it != m_removeEdges.end()) ? it->second : nullptr;
}

void Rhygine::Archetype::SetRemoveEdge(TypeId t_type, Archetype* t_target)
{
	m_removeEdges[t_type] = t_target;
}
