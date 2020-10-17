#include "GameObject.h"
#include "Updatable.h"
#include "Drawable.h"

#include <memory>

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

