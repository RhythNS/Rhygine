#include "GameObject.h"
#include "Updatable.h"
#include "Drawable.h"

#include <memory>
#include <assert.h>

GameObject::GameObject(Stage* stage) : stage(stage) {}

void GameObject::EnableComponent(Component* component)
{
	assert(component);
	// Check to see if the component is in the component list.
	if (!IsInUniqueVector<Component>(&components, component))
		return;

	// If it is an updatable and it was not already in the updateable list, add it back to it.
	Updatable* updatable;
	if ((updatable = dynamic_cast<Updatable*>(component)) &&
		!IsInVector<Updatable>(&updatables, updatable))
		updatables.push_back(updatable);

	// The same is done if it is a drawable.
	Drawable* drawable;
	if ((drawable = dynamic_cast<Drawable*>(component)) &&
		!IsInVector<Drawable>(&drawables, drawable))
		drawables.push_back(drawable);

	// change the enabled bool to true.
	component->enabled = true;
	component->OnEnabled();
}

void GameObject::DisableComponent(Component* component)
{
	assert(component);
	// Check if the component is in the component list.
	if (!IsInUniqueVector<Component>(&components, component))
		return;

	// If it is an updatable remove it from the list.
	Updatable* updatable;
	if (updatable = dynamic_cast<Updatable*>(component))
		std::erase(updatables, updatable);

	// If it is a drawable remove it from the list.
	Drawable* drawable;
	if (drawable = dynamic_cast<Drawable*>(component))
		std::erase(drawables, drawable);

	// Set the enabled bool to false.
	component->enabled = false;
	component->OnDisabled();
}

bool GameObject::RemoveComponent(Component* toRemove)
{
	// Go through each component and check if it is the component to be removed.
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i].get() == toRemove)
		{
			// If it is also an updatable, remove it from the updatable list.
			Updatable* updatable;
			if (updatable = dynamic_cast<Updatable*>(toRemove))
				std::erase(updatables, updatable);

			// If its a drawable also remove it from the drawable list.
			Drawable* drawable;
			if (drawable = dynamic_cast<Drawable*>(toRemove))
				std::erase(drawables, drawable);

			toRemove->OnRemove();

			// Lastly removed the component from the main list and return true.
			components.erase(components.begin() + i);
			return true;
		}
	}

	// Component was not found, so just return false.
	return false;
}

void GameObject::OnRemove()
{
	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->OnRemove();
	}
}

int GameObject::GetComponentCount()
{
	return (int)components.size();
}

Component* GameObject::GetComponentAt(int at)
{
	assert(at > -1 && at < components.size());
	return components[at].get();
}

std::weak_ptr<Coroutine> GameObject::StartCoroutine(Yielder yielder, std::function<void()> onFinish)
{
	coroutines.push_back(std::shared_ptr<Coroutine>(new Coroutine(yielder, this, onFinish)));
	return std::weak_ptr<Coroutine>(coroutines[coroutines.size() - 1]);
}

void GameObject::RemoveCoroutine(Coroutine* coroutine)
{
	int size = static_cast<int>(coroutines.size());
	for (int i = 0; i < size; i++)
	{
		if (coroutines[i].get() == coroutine)
		{
			coroutines.erase(coroutines.begin() + i);
			return;
		}
	}
}

Stage* GameObject::GetStage()
{
	return stage;
}

void GameObject::Init()
{
}

void GameObject::Update()
{
	for (int i = static_cast<int>(coroutines.size()) - 1; i >= 0; i--)
	{
		if (coroutines[i]->Update())
		{
			coroutines.erase(coroutines.begin() + i);
		}
	}

	for (auto& updatable : updatables)
	{
		updatable->Update();
	}
}

void GameObject::LateUpdate()
{
	for (auto& late : lateUpdatables)
	{
		late->LateUpdate();
	}
}

void GameObject::Draw()
{
	for (auto& drawable : drawables)
	{
		drawable->Draw();
	}
}
