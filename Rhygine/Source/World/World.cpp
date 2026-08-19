#include "World.h"

#include "TypeRegistration.h"
#include "Debug/Error.h"
#include "SceneRenderer.h"
#include "IDevice.h"
#include "Behaviour.h"

Rhygine::World::World(SceneRenderer& t_sceneRenderer, IDevice& t_device)
	: m_sceneRenderer(t_sceneRenderer), m_device(t_device), m_resourceManager(t_sceneRenderer, t_device)
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

void Rhygine::World::Tick(DeltaTime t_deltaTime)
{
	auto forEachBehaviour = [&](auto&& memFn, DeltaTime t_deltaTime)
		{
			for (auto& [id, behaviours] : m_behaviours)
			{
				Entity entity(id, *this);
				for (auto& behaviour : behaviours)
				{
					((*behaviour).*memFn)(entity, t_deltaTime);
				}
			}
		};

	m_inForEach = true;
	for (auto& service : m_services)
	{
		service->OnPreTick(*this, t_deltaTime);
	}
	forEachBehaviour(&Behaviour::OnPreTick, t_deltaTime);
	m_deferredCommandBuffer.Flush(*this);
	m_inForEach = false;

	m_inParallelPhase = true;
	// while accumulated time > fixed timestep
	{
		DeltaTime fixedDeltaTime = 1.0f / 60.0f;
		// TODO: Call physics engine tick here
		for (auto& service : m_services)
		{
			service->OnPostPhysicsTick(*this, t_deltaTime);
		}
		forEachBehaviour(&Behaviour::OnPostPhysicsTick, fixedDeltaTime);
		// accumulatedTime -= fixedDeltaTime;
	}
	m_inParallelPhase = false;
	m_deferredCommandBuffer.Flush(*this);

	m_inParallelPhase = true;
	// TODO: Normal Tasks
	m_inParallelPhase = false;

	m_inForEach = true;
	for (auto& service : m_services)
	{
		service->OnPostTick(*this, t_deltaTime);
	}
	forEachBehaviour(&Behaviour::OnPostPhysicsTick, t_deltaTime);
	m_deferredCommandBuffer.Flush(*this);
	m_inForEach = false;

	m_inForEach = true;

	for (auto& service : m_services)
	{
		service->OnPostTick(*this, t_deltaTime);
	}
	forEachBehaviour(&Behaviour::OnPostTick, t_deltaTime);
	m_deferredCommandBuffer.Flush(*this);
	m_inForEach = false;

	if (m_toRemoveServices.size() > 0)
	{
		for (Service* service : m_toRemoveServices)
		{
			service->OnShutdown(*this);

			auto it = std::find_if(m_services.begin(), m_services.end(), [service](const std::unique_ptr<Service>& s) {
				return s.get() == service;
				});
			if (it != m_services.end())
			{
				m_services.erase(it);
			}
		}
		m_toRemoveServices.clear();
	}
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

void* Rhygine::World::AddComponent(EntityId t_entityId, TypeId t_type, const void* t_data)
{
	if (IsAlive(t_entityId) == false)
	{
		return nullptr;
	}

	return InnerAddComponent(t_entityId, t_type, t_data);
}

void* Rhygine::World::GetComponent(EntityId t_entityId, TypeId t_type) const
{
	if (IsAlive(t_entityId) == false)
	{
		return nullptr;
	}

	return InnerGetComponent(t_entityId, t_type);
}

bool Rhygine::World::HasComponent(EntityId t_entityId, TypeId t_type) const
{
	if (IsAlive(t_entityId) == false)
	{
		return false;
	}
	return InnerHasComponent(t_entityId, t_type);
}

bool Rhygine::World::RemoveComponent(EntityId t_entityId, TypeId t_type)
{
	if (IsAlive(t_entityId) == false)
	{
		return false;
	}

	return InnerRemoveComponent(t_entityId, t_type);
}

void Rhygine::World::AddBehaviour(EntityId t_entityId, TypeId t_type, void* t_args)
{
	if (IsAlive(t_entityId) == false)
	{
		return;
	}

	InnerAddBehaviour(t_entityId, t_type, t_args);
}

Rhygine::Behaviour* Rhygine::World::GetBehaviour(EntityId t_entityId, TypeId t_type) const
{
	if (IsAlive(t_entityId) == false)
	{
		return nullptr;
	}

	return InnerGetBehaviour(t_entityId, t_type);
}

bool Rhygine::World::HasBehaviour(EntityId t_entityId, TypeId t_type) const
{
	if (IsAlive(t_entityId) == false)
	{
		return false;
	}

	return InnerHasBehaviour(t_entityId, t_type);
}

bool Rhygine::World::RemoveBehaviour(EntityId t_entityId, TypeId t_type)
{
	if (IsAlive(t_entityId) == false)
	{
		return false;
	}
	return InnerRemoveBehaviour(t_entityId, t_type);
}

void Rhygine::World::BuildRenderPacket(RenderPacketBuilder& t_builder) const
{
	for (Archetype* archetype : m_extractorArchetypes)
	{
		archetype->BuildRenderPacket(t_builder, m_TypeRegistry);
	}
}

Rhygine::ResourceManager& Rhygine::World::GetResourceManager()
{
	return m_resourceManager;
}

Rhygine::Service* Rhygine::World::AddService(TypeId t_type, void* t_args)
{
	TypeInfo typeInfo = m_TypeRegistry.GetTypeInfo(t_type);
	Service* existingService = GetService(t_type);
	if (existingService)
	{
		return existingService;
	}
	std::unique_ptr<Service> service(static_cast<Service*>(typeInfo.factory(t_args).release()));
	service->m_typeId = t_type;
	m_services.push_back(std::move(service));
	return m_services.back().get();
}

Rhygine::Service* Rhygine::World::GetService(TypeId t_type)
{
	auto it = std::find_if(m_services.begin(), m_services.end(), [t_type](const std::unique_ptr<Service>& service)
		{
			return service->m_typeId == t_type;
		});
	if (it != m_services.end())
	{
		return it->get();
	}
	return nullptr;
}

bool Rhygine::World::HasService(TypeId t_type) const
{
	return std::any_of(m_services.begin(), m_services.end(), [t_type](const std::unique_ptr<Service>& service)
		{
			return service->m_typeId == t_type;
		});
}

bool Rhygine::World::RemoveService(TypeId t_type)
{
	Service* service = GetService(t_type);
	if (!service)
	{
		return false;
	}
	m_toRemoveServices.push_back(service);
	return true;
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

Rhygine::Behaviour* Rhygine::World::InnerAddBehaviour(EntityId t_entityId, TypeId t_type, void* t_args)
{
	Rhygine::Behaviour* existingBehaviour = InnerGetBehaviour(t_entityId, t_type);
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

Rhygine::Behaviour* Rhygine::World::InnerGetBehaviour(EntityId t_entityId, TypeId t_type) const
{
	auto it = m_behaviours.find(t_entityId);
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
			STOP_EXECUTION_MESSAGE("Entity can not have Task dependencies.");
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

Rhygine::Behaviour* Rhygine::World::AddBehaviourImmediate(EntityId t_id, TypeId t_type, void* t_args)
{
	TypeInfo typeInfo = m_TypeRegistry.GetTypeInfo(t_type);
	std::unique_ptr<Behaviour> behaviour(static_cast<Behaviour*>(typeInfo.factory(t_args).release()));
	auto it = m_behaviours.find(t_id);
	if (it == m_behaviours.end())
	{
		m_behaviours[t_id] = std::vector<std::unique_ptr<Behaviour>>();
		it = m_behaviours.find(t_id);
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

	auto behaviourIt = m_behaviours.find(t_id);
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

	auto it = m_behaviours.find(t_id);
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
