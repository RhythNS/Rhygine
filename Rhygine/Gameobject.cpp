#include "GameObject.h"
#include "Updatable.h"
#include "Drawable.h"

#include <memory>

void GameObject::EnableComponent(Component* component)
{
	if (!IsInUniqueVector<Component>(&components, component))
		return;

	Updatable* updatable;
	if ((updatable = dynamic_cast<Updatable*>(component)) &&
		!IsInVector<Updatable>(&updatables, updatable))
		updatables.push_back(updatable);

	Drawable* drawable;
	if ((drawable = dynamic_cast<Drawable*>(component)) &&
		!IsInVector<Drawable>(&drawables, drawable))
		drawables.push_back(drawable);

	component->enabled = true;
}

void GameObject::DisableComponent(Component* component)
{
	if (!IsInUniqueVector<Component>(&components, component))
		return;

	Updatable* updatable;
	if (updatable = dynamic_cast<Updatable*>(component))
		std::erase(updatables, updatable);

	Drawable* drawable;
	if (drawable = dynamic_cast<Drawable*>(component))
		std::erase(drawables, drawable);

	component->enabled = false;
}

bool GameObject::RemoveComponent(Component* toRemove)
{
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i].get() == toRemove)
		{
			Updatable* updatable;
			if (updatable = dynamic_cast<Updatable*>(toRemove))
				std::erase(updatables, updatable);

			Drawable* drawable;
			if (drawable = dynamic_cast<Drawable*>(toRemove))
				std::erase(drawables, drawable);

			components.erase(components.begin() + i);
			return true;
		}
	}
	return false;
}

int GameObject::GetComponentCount()
{
	return (int)components.size();
}

Component* GameObject::GetComponentAt(int at)
{
	return components[at].get();
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
	for (auto& updatable : updatables)
	{
		updatable->Update();
	}
}

void GameObject::Draw()
{
	for (auto& drawable : drawables)
	{
		drawable->Draw();
	}
}
