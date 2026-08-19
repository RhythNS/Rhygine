#include "World.h"

#include "TypeRegistration.h"
#include "Debug/Error.h"

Rhygine::World::World()
{
	m_entityRecords.reserve(1000);

	auto& pendingRegistrations = detail::PendingRegistration::s_pendingRegistrations;
	for (auto& registration : pendingRegistrations)
	{
		if (registration.registerFn)
		{
			registration.registerFn(m_TypeRegistry);
		}
	}

	for (auto& registration : pendingRegistrations)
	{
		if (registration.parentChildFn)
		{
			registration.parentChildFn(m_TypeRegistry);
		}
		if (registration.dependencyFn)
		{
			registration.dependencyFn(m_TypeRegistry);
		}
	}

	pendingRegistrations.clear();

	m_archetypes.reserve(100);
	m_archetypes[ArchetypeSignature::Empty()] = std::make_unique<Archetype>(ArchetypeSignature::Empty(), m_TypeRegistry);
}

Rhygine::EntityId Rhygine::World::CreateEntity(SceneId t_sceneId)
{
	EntityId newEntityId = EntityId::Invalid();

	if (m_freeEntityIndices.empty())
	{
		newEntityId.index = static_cast<uint32_t>(m_entityRecords.size());
		newEntityId.generation = 1;
		m_entityRecords.push_back({ newEntityId.generation, nullptr, 0, t_sceneId, false });
	}
	else
	{
		newEntityId.index = m_freeEntityIndices.front();
		m_freeEntityIndices.pop();
		newEntityId.generation = m_entityRecords[newEntityId.index].generation;
		m_entityRecords[newEntityId.index] = { newEntityId.generation, nullptr, 0, t_sceneId, false };
	}

	if (ShouldDeferCommands())
	{
		m_deferredCommandBuffer.QueueCreateEntity(newEntityId);
	}
	else
	{
		CreateEntityImmediate(newEntityId);
	}

	return newEntityId;
}

void Rhygine::World::DestroyEntity(EntityId t_entityId)
{
	if (IsAlive(t_entityId) == false || m_entityRecords[t_entityId.index].markedForDeletion)
	{
		return;
	}

	m_entityRecords[t_entityId.index].markedForDeletion = true;
	m_deferredCommandBuffer.QueueDestroyEntity(t_entityId);
}

bool Rhygine::World::IsAlive(EntityId t_entityId) const
{
	return m_entityRecords.size() > t_entityId.index &&
		m_entityRecords[t_entityId.index].generation == t_entityId.generation &&
		m_entityRecords[t_entityId.index].archetype != nullptr;
}

template<typename T>
inline T* Rhygine::World::AddComponent(EntityId t_entityId, const void* t_data)
{
	TypeId id = m_TypeRegistry.GetComponentTypeId<T>();
	void* component = AddComponent(id, t_entityId, t_data);
	return static_cast<T*>(component);
}

void* Rhygine::World::AddComponent(EntityId t_entityId, TypeId t_type, const void* t_data)
{
	if (IsAlive(t_entityId) == false)
	{
		return nullptr;
	}

	return InnerAddComponent(t_entityId, t_type, t_data);
}

template<typename T>
T* Rhygine::World::GetComponent(EntityId t_entityId) const
{
	TypeId id = m_TypeRegistry.GetComponentTypeId<T>();
	void* component = GetComponent(t_entityId, id);
	return static_cast<T*>(component);
}

void* Rhygine::World::GetComponent(EntityId t_entityId, TypeId t_type) const
{
	if (IsAlive(t_entityId) == false)
	{
		return nullptr;
	}

	return InnerGetComponent(t_entityId, t_type);
}

template<typename T>
bool Rhygine::World::HasComponent(EntityId t_entityId) const
{
	TypeId id = m_TypeRegistry.GetComponentTypeId<T>();
	return HasComponent(t_entityId, id);
}

bool Rhygine::World::HasComponent(EntityId t_entityId, TypeId t_type) const
{
	if (IsAlive(t_entityId) == false)
	{
		return false;
	}
	return InnerHasComponent(t_entityId, t_type);
}

template<typename T>
bool Rhygine::World::RemoveComponent(EntityId t_entityId)
{
	TypeId id = m_TypeRegistry.GetComponentTypeId<T>();
	return RemoveComponent(t_entityId, id);
}

bool Rhygine::World::RemoveComponent(EntityId t_entityId, TypeId t_type)
{
	if (IsAlive(t_entityId) == false)
	{
		return false;
	}

	return InnerRemoveComponent(t_entityId, t_type);
}

template<typename T, typename ...Args>
void Rhygine::World::AddBehaviour(EntityId t_entityId, Args && ...t_args)
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

void Rhygine::World::AddBehaviour(EntityId t_entityId, TypeId t_type, void* t_args)
{
	if (IsAlive(t_entityId) == false)
	{
		return;
	}

	InnerAddBehaviour(t_entityId, t_type, t_args);
}

template<typename T>
T* Rhygine::World::GetBehaviour(EntityId t_entityId) const
{
	static TypeId id = m_TypeRegistry.GetBehaviourTypeId<T>();
	return static_cast<T*>(GetBehaviour(t_entityId, id));
}

void* Rhygine::World::GetBehaviour(EntityId t_entityId, TypeId t_type) const
{
	if (IsAlive(t_entityId) == false)
	{
		return nullptr;
	}

	return InnerGetBehaviour(t_entityId, t_type);
}

template<typename T>
bool Rhygine::World::HasBehaviour(EntityId t_entityId) const
{
	static TypeId id = m_TypeRegistry.GetBehaviourTypeId<T>();
	return HasBehaviour(t_entityId, id);
}

bool Rhygine::World::HasBehaviour(EntityId t_entityId, TypeId t_type) const
{
	if (IsAlive(t_entityId) == false)
	{
		return false;
	}

	return InnerHasBehaviour(t_entityId, t_type);
}

template<typename T>
bool Rhygine::World::RemoveBehaviour(EntityId t_entityId)
{
	static TypeId id = m_TypeRegistry.GetBehaviourTypeId<T>();
	return RemoveBehaviour(t_entityId, id);
}

bool Rhygine::World::RemoveBehaviour(EntityId t_entityId, TypeId t_type)
{
	if (IsAlive(t_entityId) == false)
	{
		return false;
	}
	return InnerRemoveBehaviour(t_entityId, t_type);
}

template<typename ...Ts>
void Rhygine::World::ForEach(std::function<void(EntityId, Ts&...)> t_fn)
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

void Rhygine::World::BuildRenderPacket(RenderPacketBuilder& t_builder) const
{
	for (Archetype* archetype : m_extractorArchetypes)
	{
		archetype->BuildRenderPacket(t_builder, m_TypeRegistry);
	}
}

void* Rhygine::World::InnerAddComponent(EntityId t_entityId, TypeId t_type, const void* t_data)
{
	void* existingComponent = InnerGetComponent(t_entityId, t_type);
	if (existingComponent != nullptr)
	{
		return existingComponent;
	}

	InnerAddTypeDependencies(t_entityId, t_type);

	if (ShouldDeferCommands())
	{
		const TypeInfo& typeInfo = m_TypeRegistry.GetTypeInfo(t_type);
		std::unique_ptr<std::byte[]> dataCopy = std::make_unique<std::byte[]>(typeInfo.size);
		std::memcpy(dataCopy.get(), t_data, typeInfo.size);
		m_deferredCommandBuffer.QueueAddComponent(t_entityId, t_type, std::move(dataCopy));
		return nullptr;
	}

	return AddComponentImmediate(t_entityId, t_type, t_data);
}

void* Rhygine::World::InnerGetComponent(EntityId t_entityId, TypeId t_type) const
{
	EntityRecord record = m_entityRecords[t_entityId.index];
	return record.archetype->GetComponentData(t_type, record.rowInArchetype);
}

bool Rhygine::World::InnerHasComponent(EntityId t_entityId, TypeId t_type) const
{
	return m_entityRecords[t_entityId.index].archetype->ContainsComponent(t_type);
}

bool Rhygine::World::InnerRemoveComponent(EntityId t_entityId, TypeId t_type)
{
	if (m_entityRecords[t_entityId.index].archetype->ContainsComponent(t_type) == false)
	{
		return false;
	}

	InnerRemoveTypeDependents(t_entityId, t_type);

	m_deferredCommandBuffer.QueueRemoveComponent(t_entityId, t_type);
	return true;
}

void* Rhygine::World::InnerAddBehaviour(EntityId t_entityId, TypeId t_type, void* t_args)
{
	void* existingBehaviour = InnerGetBehaviour(t_entityId, t_type);
	if (existingBehaviour != nullptr)
	{
		return existingBehaviour;
	}

	InnerAddTypeDependencies(t_entityId, t_type);

	if (ShouldDeferCommands())
	{
		m_deferredCommandBuffer.QueueAddBehaviour(t_entityId, t_type, t_args);
		return nullptr;
	}

	return AddBehaviourImmediate(t_entityId, t_type, t_args);
}

void* Rhygine::World::InnerGetBehaviour(EntityId t_entityId, TypeId t_type) const
{
	auto it = m_behaviours.find(t_entityId.index);
	if (it != m_behaviours.end())
	{
		auto innerIt = std::find_if(it->second.begin(), it->second.end(), [t_type](const std::unique_ptr<Behaviour>& behaviour) {
			return behaviour->GetTypeId() == t_type;
			});
		if (innerIt != it->second.end())
		{
			return innerIt->get();
		}
	}
	return nullptr;
}

bool Rhygine::World::InnerHasBehaviour(EntityId t_entityId, TypeId t_type) const
{
	return InnerGetBehaviour(t_entityId, t_type) != nullptr;
}

bool Rhygine::World::InnerRemoveBehaviour(EntityId t_entityId, TypeId t_type)
{
	if (HasBehaviour(t_entityId, t_type) == false)
	{
		return false;
	}

	InnerRemoveTypeDependents(t_entityId, t_type);

	m_deferredCommandBuffer.QueueRemoveBehaviour(t_entityId, t_type);
	return true;
}

void Rhygine::World::InnerAddTypeDependencies(EntityId t_entityId, TypeId t_type)
{
	auto& dependencies = m_TypeRegistry.GetDependencies(t_type);
	for (TypeId depType : dependencies)
	{
		TypeInfo typeInfo = m_TypeRegistry.GetTypeInfo(depType);

		// TODO: Handle dependencies for Behaviours, Services, and Tasks
		switch (typeInfo.kind)
		{
		case TypeKind::Component:
			if (InnerHasComponent(t_entityId, depType) == false)
			{
				InnerAddComponent(t_entityId, depType, nullptr);
			}
			break;
		case TypeKind::Behaviour:
			if (InnerHasBehaviour(t_entityId, depType) == false)
			{
				InnerAddBehaviour(t_entityId, depType, nullptr);
			}
			break;
		case TypeKind::Service:
			STOP_EXECUTION_MESSAGE("Entity can not have Service dependencies.");
			break;
		case TypeKind::Task:
			break;
		default:
			STOP_EXECUTION_MESSAGE("Unknown TypeKind encountered in InnerAddTypeDependencies.");
			break;
		}
	}
}

void Rhygine::World::InnerRemoveTypeDependents(EntityId t_entityId, TypeId t_type)
{
	auto& dependencies = m_TypeRegistry.GetDependents(t_type);
	for (TypeId dependentType : dependencies)
	{
		TypeInfo typeInfo = m_TypeRegistry.GetTypeInfo(dependentType);

		// TODO: Handle dependencies for Behaviours, Services, and Tasks
		switch (typeInfo.kind)
		{
		case TypeKind::Component:
			if (InnerHasComponent(t_entityId, dependentType) == true)
			{
				InnerRemoveComponent(t_entityId, dependentType);
			}
			break;
		case TypeKind::Behaviour:
			if (InnerHasBehaviour(t_entityId, dependentType) == true)
			{
				InnerRemoveBehaviour(t_entityId, dependentType);
			}
			break;
		case TypeKind::Service:
			STOP_EXECUTION_MESSAGE("Entity can not have Service dependencies.");
			break;
		case TypeKind::Task:
			break;
		default:
			STOP_EXECUTION_MESSAGE("Unknown TypeKind encountered in InnerAddTypeDependencies.");
			break;
		}
	}
}

void Rhygine::World::CreateEntityImmediate(EntityId t_id)
{
	ASSERT_ERROR_MESSAGE(IsAlive(t_id) == false, "Entity is already alive.");
	// TODO: Check if scene exists and is valid

	Archetype* emptyArchetype = m_archetypes[ArchetypeSignature::Empty()].get();
	size_t index = emptyArchetype->AllocateEntity(t_id);

	EntityRecord record = m_entityRecords[t_id.index];
	record.archetype = emptyArchetype;
	record.rowInArchetype = index;
}

void* Rhygine::World::AddComponentImmediate(EntityId t_id, TypeId t_type, const void* t_data)
{
	if (IsAlive(t_id) == false)
	{
		return nullptr;
	}

	EntityRecord record = m_entityRecords[t_id.index];
	Archetype* currentArchetype = record.archetype;
	const TypeInfo& typeInfo = m_TypeRegistry.GetTypeInfo(t_type);
	ASSERT_ERROR_MESSAGE(typeInfo.kind == TypeKind::Component, "Type is not a component.");
	ASSERT_ERROR_MESSAGE(currentArchetype->ContainsComponent(t_type) == false, "Entity already has component of this type.");
	Archetype* nextArchetype = record.archetype->GetAddEdge(t_type);
	if (nextArchetype == nullptr)
	{
		ArchetypeSignature newSignature = currentArchetype->GetSignature() + t_type;
		nextArchetype = &GetOrCreateArchetype(newSignature);
		currentArchetype->SetAddEdge(t_type, nextArchetype);
	}
	auto movedResult = currentArchetype->MoveEntityTo(record.rowInArchetype, *nextArchetype, t_type, const_cast<void*>(t_data));
	record.archetype = nextArchetype;
	record.rowInArchetype = movedResult.targetRow;

	if (movedResult.sourceMovedEntity.has_value())
	{
		m_entityRecords[movedResult.sourceMovedEntity->entity.index].rowInArchetype = movedResult.sourceMovedEntity->newRow;
	}

	void* componentData = nextArchetype->GetComponentData(t_type, movedResult.targetRow);
	if (typeInfo.attach)
	{
		typeInfo.attach(t_id, componentData, *this);
	}

	return componentData;
}

void* Rhygine::World::AddBehaviourImmediate(EntityId t_id, TypeId t_type, void* t_args)
{
	TypeInfo typeInfo = m_TypeRegistry.GetTypeInfo(t_type);
	std::unique_ptr<Behaviour> behaviour(static_cast<Behaviour*>(typeInfo.factory(t_args).release()));
	auto it = m_behaviours.find(t_id.index);
	if (it == m_behaviours.end())
	{
		m_behaviours[t_id.index] = std::vector<std::unique_ptr<Behaviour>>();
		it = m_behaviours.find(t_id.index);
	}
	it->second.push_back(std::move(behaviour));
	return it->second.back().get();
}

void Rhygine::World::DestroyEntityImmediate(EntityId t_id)
{
	EntityRecord record = m_entityRecords[t_id.index];
	if (record.archetype == nullptr)
	{
		return;
	}

	auto behaviourIt = m_behaviours.find(t_id.index);
	if (behaviourIt != m_behaviours.end())
	{
		for (auto& behaviour : behaviourIt->second)
		{
			behaviour->OnDestroy(Entity(t_id, (*this)));
		}
		m_behaviours.erase(behaviourIt);

		// Reset the record after calling OnDestroy in case the behaviour modified components or the entity itself
		record = m_entityRecords[t_id.index];
	}

	for (TypeId componentType : record.archetype->GetSignature().GetComponentTypes())
	{
		const TypeInfo& typeInfo = m_TypeRegistry.GetTypeInfo(componentType);
		if (typeInfo.detach)
		{
			void* componentData = record.archetype->GetComponentData(componentType, record.rowInArchetype);
			typeInfo.detach(t_id, componentData, *this);
		}
	}

	auto movedEntity = record.archetype->RemoveEntityByRow(record.rowInArchetype);
	if (movedEntity.has_value())
	{
		m_entityRecords[movedEntity->entity.index].rowInArchetype = movedEntity->newRow;
	}

	record.archetype = nullptr;
	record.generation++;
	record.markedForDeletion = false;

	m_freeEntityIndices.push(t_id.index);
}

void Rhygine::World::RemoveComponentImmediate(EntityId t_id, TypeId t_type)
{
	if (IsAlive(t_id) == false)
	{
		return;
	}

	EntityRecord record = m_entityRecords[t_id.index];

	const TypeInfo& typeInfo = m_TypeRegistry.GetTypeInfo(t_type);
	if (typeInfo.detach)
	{
		void* componentData = record.archetype->GetComponentData(t_type, record.rowInArchetype);
		typeInfo.detach(t_id, componentData, *this);
	}

	Archetype* currentArchetype = record.archetype;

	if (currentArchetype->GetSignature().Contains(t_type) == false)
	{
		return;
	}

	Archetype* nextArchetype = record.archetype->GetAddEdge(t_type);
	if (nextArchetype == nullptr)
	{
		ArchetypeSignature newSignature = currentArchetype->GetSignature() - t_type;
		nextArchetype = &GetOrCreateArchetype(newSignature);
		currentArchetype->SetAddEdge(t_type, nextArchetype);
	}
	auto movedResult = currentArchetype->MoveEntityTo(record.rowInArchetype, *nextArchetype, t_type);
	record.archetype = nextArchetype;
	record.rowInArchetype = movedResult.targetRow;

	if (movedResult.sourceMovedEntity.has_value())
	{
		m_entityRecords[movedResult.sourceMovedEntity->entity.index].rowInArchetype = movedResult.sourceMovedEntity->newRow;
	}
}

void Rhygine::World::RemoveBehaviourImmediate(EntityId t_id, TypeId t_type)
{
	if (IsAlive(t_id) == false)
	{
		return;
	}

	auto it = m_behaviours.find(t_id.index);
	if (it != m_behaviours.end())
	{
		auto& behaviours = it->second;
		behaviours.erase(std::remove_if(behaviours.begin(), behaviours.end(), [&](const std::unique_ptr<Behaviour>& behaviour)
			{
				if (behaviour->GetTypeId() == t_type)
				{
					behaviour->OnDestroy(Entity(t_id, (*this)));
					return true;

				}
				else
				{
					return false;
				}
			}), behaviours.end());

		if (behaviours.empty())
		{
			m_behaviours.erase(it);
		}
	}
}

Rhygine::Archetype& Rhygine::World::GetOrCreateArchetype(const ArchetypeSignature& t_signature)
{
	auto it = m_archetypes.find(t_signature);
	if (it != m_archetypes.end())
	{
		return *(it->second);
	}
	std::unique_ptr<Archetype> archetype = std::make_unique<Archetype>(t_signature, m_TypeRegistry);
	Archetype& ref = *archetype;
	m_archetypes[t_signature] = std::move(archetype);
	if (ref.NeedsExtraction())
	{
		m_extractorArchetypes.push_back(&ref);
	}
	return ref;
}
