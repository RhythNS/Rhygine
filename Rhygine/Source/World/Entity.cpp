#include "Entity.h"

#include "World.h"

template<typename T>
inline T* Rhygine::Entity::AddComponent(T&& t_data)
{
	return m_world.AddComponent<T>(m_id, &t_data);
}

void* Rhygine::Entity::AddComponent(TypeId t_type, const void* t_data)
{
	return m_world.AddComponent(m_id, t_type, t_data);
}

template<typename T>
T* Rhygine::Entity::GetComponent() const
{
	return m_world.GetComponent<T>(m_id);
}

void* Rhygine::Entity::GetComponent(TypeId t_type) const
{
	return m_world.GetComponent(m_id, t_type);
}

template<typename T>
bool Rhygine::Entity::HasComponent() const
{
	return m_world.HasComponent<T>(m_id);
}

bool Rhygine::Entity::HasComponent(TypeId t_type) const
{
	return m_world.HasComponent(m_id, t_type);
}

template<typename T>
bool Rhygine::Entity::RemoveComponent()
{
	return m_world.RemoveComponent<T>(m_id);
}

bool Rhygine::Entity::RemoveComponent(TypeId t_type)
{
	return m_world.RemoveComponent(m_id, t_type);
}

template<typename T, typename ...Args>
void Rhygine::Entity::AddBehaviour(Args && ...t_args)
{
	m_world.AddBehaviour<T>(m_id, std::forward<Args>(t_args)...);
}

void Rhygine::Entity::AddBehaviour(TypeId t_type, void* t_args)
{
	m_world.AddBehaviour(m_id, t_type, t_args);
}

template<typename T>
T* Rhygine::Entity::GetBehaviour() const
{
	return m_world.GetBehaviour<T>(m_id);
}

void* Rhygine::Entity::GetBehaviour(TypeId t_type) const
{
	return m_world.GetBehaviour(m_id, t_type);
}

template<typename T>
bool Rhygine::Entity::HasBehaviour() const
{
	return m_world.HasBehaviour<T>(m_id);
}

bool Rhygine::Entity::HasBehaviour(TypeId t_type) const
{
	return m_world.HasBehaviour(m_id, t_type);
}

template<typename T>
bool Rhygine::Entity::RemoveBehaviour()
{
	return m_world.RemoveBehaviour<T>(m_id);
}

bool Rhygine::Entity::RemoveBehaviour(TypeId t_type)
{
	return m_world.RemoveBehaviour(m_id, t_type);
}

bool Rhygine::Entity::IsAlive() const
{
	return m_world.IsAlive(m_id);
}
