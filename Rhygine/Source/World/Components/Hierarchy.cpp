#include "Hierarchy.h"

#include "Debug/Error.h"

void Rhygine::Hierarchy::AddChild(EntityId t_child, World& t_world)
{
	Hierarchy* childHierarchy = t_world.GetComponent<Hierarchy>(t_child);
	ASSERT_ERROR_MESSAGE(childHierarchy != nullptr, "Child entity does not have a Hierarchy component.");
	if (childHierarchy->parent != EntityId::Invalid())
	{
		Hierarchy* oldParentHierarchy = t_world.GetComponent<Hierarchy>(childHierarchy->parent);
		ASSERT_ERROR_MESSAGE(oldParentHierarchy != nullptr, "Old parent entity does not have a Hierarchy component.");
		oldParentHierarchy->RemoveChild(t_child, t_world);
	}

	if (firstChild == EntityId::Invalid())
	{
		firstChild = t_child;
		childHierarchy->previousSibling = EntityId::Invalid();
		childHierarchy->nextSibling = EntityId::Invalid();
	}
	else
	{
		Hierarchy* firstChildHierarchy = t_world.GetComponent<Hierarchy>(firstChild);
		ASSERT_ERROR_MESSAGE(firstChildHierarchy != nullptr, "First child entity does not have a Hierarchy component.");
		childHierarchy->nextSibling = firstChild;
		firstChildHierarchy->previousSibling = t_child;
		firstChild = t_child;
	}
}

void Rhygine::Hierarchy::RemoveChild(EntityId t_child, World& t_world)
{
	Hierarchy* childHierarchy = t_world.GetComponent<Hierarchy>(t_child);
	ASSERT_ERROR_MESSAGE(childHierarchy != nullptr, "Child entity does not have a Hierarchy component.");

	if (childHierarchy->previousSibling != EntityId::Invalid())
	{
		Hierarchy* previousSiblingHierarchy = t_world.GetComponent<Hierarchy>(childHierarchy->previousSibling);
		ASSERT_ERROR_MESSAGE(previousSiblingHierarchy != nullptr, "Previous sibling entity does not have a Hierarchy component.");
		previousSiblingHierarchy->nextSibling = childHierarchy->nextSibling;
	}
	else
	{
		firstChild = childHierarchy->nextSibling;
	}

	if (childHierarchy->nextSibling != EntityId::Invalid())
	{
		Hierarchy* nextSiblingHierarchy = t_world.GetComponent<Hierarchy>(childHierarchy->nextSibling);
		ASSERT_ERROR_MESSAGE(nextSiblingHierarchy != nullptr, "Next sibling entity does not have a Hierarchy component.");
		nextSiblingHierarchy->previousSibling = childHierarchy->previousSibling;
	}

	childHierarchy->parent = EntityId::Invalid();
	childHierarchy->previousSibling = EntityId::Invalid();
	childHierarchy->nextSibling = EntityId::Invalid();
}

void Rhygine::Hierarchy_OnDetach(EntityId t_entityId, void* t_data, World& t_world)
{
	Hierarchy* hierarchy = static_cast<Hierarchy*>(t_data);
	ASSERT_ERROR_MESSAGE(hierarchy != nullptr, "Hierarchy component data is null.");

	if (hierarchy->m_skipDetach)
	{
		return;
	}

	if (hierarchy->parent != EntityId::Invalid())
	{
		Hierarchy* parentHierarchy = t_world.GetComponent<Hierarchy>(hierarchy->parent);
		ASSERT_ERROR_MESSAGE(parentHierarchy != nullptr, "Parent entity does not have a Hierarchy component.");
		parentHierarchy->RemoveChild(t_entityId, t_world);
	}

	hierarchy->DeleteChildren(t_world);
}

void Rhygine::Hierarchy::DeleteChildren(World& t_world)
{
	EntityId currentId = firstChild;
	while (currentId != EntityId::Invalid())
	{
		Hierarchy* current = t_world.GetComponent<Hierarchy>(currentId);
		ASSERT_ERROR_MESSAGE(current != nullptr, "Child entity does not have a Hierarchy component.");
		
		current->m_skipDetach = true;
		current->DeleteChildren(t_world);
		
		EntityId next = current->nextSibling;
		t_world.DestroyEntity(currentId);
		currentId = next;
	}
}
