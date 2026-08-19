#include "TypeRegistry.h"

Rhygine::TypeId Rhygine::TypeRegistry::Register(const TypeInfo& t_info)
{
	ZoneScoped;
	ASSERT_ERROR_MESSAGE(m_nameToId.find(t_info.name) == m_nameToId.end(), "Type already registered: " + t_info.name);

	TypeId newId = m_nextId++;
	m_types.push_back(t_info);
	m_nameToId[t_info.name] = newId;
	return newId;
}

std::optional<Rhygine::TypeId> Rhygine::TypeRegistry::FindByName(const std::string& t_name) const
{
	ZoneScoped;

	auto it = m_nameToId.find(t_name);
	if (it != m_nameToId.end())
	{
		return it->second;
	}
	return std::nullopt;
}

const Rhygine::TypeInfo& Rhygine::TypeRegistry::GetTypeInfo(TypeId t_id) const
{
	ZoneScoped;

	ASSERT_ERROR_MESSAGE(t_id < m_types.size(), "Invalid TypeId");
	return m_types[t_id];
}

Rhygine::TypeKind Rhygine::TypeRegistry::GetKind(TypeId t_id) const
{
	ZoneScoped;

	return GetTypeInfo(t_id).kind;
}

void Rhygine::TypeRegistry::RegisterDependency(TypeId t_dependent, TypeId t_required)
{
	ZoneScoped;

	// TODO: Check for circular dependencies
	// TODO: Think about service dependencies, maybe they should be handled differently

	m_dependencies[t_dependent].push_back(t_required);
	m_dependents[t_required].push_back(t_dependent);
}

void Rhygine::TypeRegistry::RegisterParentChild(TypeId t_parent, TypeId t_child)
{
	ZoneScoped;

	// TODO: Check for circular dependencies
	// TODO: Think about service dependencies, maybe they should be handled differently

	m_children[t_parent].push_back(t_child);
	m_parents[t_child] = t_parent;
}

const std::vector<Rhygine::TypeId>& Rhygine::TypeRegistry::GetDependencies(TypeId t_id) const
{
	ZoneScoped;

	auto it = m_dependencies.find(t_id);
	if (it != m_dependencies.end())
	{
		return it->second;
	}
	static const std::vector<TypeId> empty;
	return empty;
}

const std::vector<Rhygine::TypeId>& Rhygine::TypeRegistry::GetDependents(TypeId t_id) const
{
	ZoneScoped;

	auto it = m_dependents.find(t_id);
	if (it != m_dependents.end())
	{
		return it->second;
	}
	static const std::vector<TypeId> empty;
	return empty;
}

const std::vector<Rhygine::TypeId>& Rhygine::TypeRegistry::GetChildren(TypeId t_id) const
{
	ZoneScoped;

	auto it = m_children.find(t_id);
	if (it != m_children.end())
	{
		return it->second;
	}
	static const std::vector<TypeId> empty;
	return empty;
}

const std::optional<Rhygine::TypeId> Rhygine::TypeRegistry::GetParent(TypeId t_id) const
{
	ZoneScoped;

	auto it = m_parents.find(t_id);
	if (it != m_parents.end())
	{
		return it->second;
	}
	return std::nullopt;
}
