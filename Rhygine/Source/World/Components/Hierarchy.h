#pragma once

#include "Types.h"
#include "Transform.h"

namespace Rhygine
{
	struct Hierarchy
	{
		friend void Hierarchy_OnDetach(EntityId t_entityId, void* t_data, World& t_world);

		EntityId parent = EntityId::Invalid();
		EntityId firstChild = EntityId::Invalid();
		EntityId previousSibling = EntityId::Invalid();
		EntityId nextSibling = EntityId::Invalid();

		void AddChild(EntityId t_child, World& t_world);
		void RemoveChild(EntityId t_child, World& t_world);

		[[nodiscard]] bool IsRoot() const { return parent == EntityId::Invalid(); }
		[[nodiscard]] bool HasChildren() const { return firstChild != EntityId::Invalid(); }
		[[nodiscard]] bool IsLeaf() const { return !HasChildren(); }
		[[nodiscard]] bool HasParent() const { return parent != EntityId::Invalid(); }
		[[nodiscard]] bool HasPreviousSibling() const { return previousSibling != EntityId::Invalid(); }
		[[nodiscard]] bool HasNextSibling() const { return nextSibling != EntityId::Invalid(); }

	private:
		bool m_skipDetach = false;

		void DeleteChildren(World& t_world);
	};

	void Hierarchy_OnDetach(EntityId t_entityId, void* t_data, World& t_world);
	
	RHYGINE_COMPONENT_WITH_ATTACH_DETACH_AND_DEPENDENCIES(Hierarchy, nullptr, &Hierarchy_OnDetach, Transform);
}
