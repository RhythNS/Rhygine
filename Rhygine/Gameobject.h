#pragma once
#include "Updatable.h"
#include "Drawable.h"
#include "Component.h"

#include <vector>
#include <memory>

class Stage;

class GameObject
{
	friend class Stage;
public:
	void Init();
	void Update();
	void Draw();

	template <class T>
	T* AddComponent()
	{
		components.push_back(std::make_unique<T>());
		T* t = static_cast<T*>(components[components.size() - 1].get());
		t->SetGameObject(this);
		t->Init();

		Updatable* updatable;
		if (updatable = dynamic_cast<Updatable*>(t))
			updatables.push_back(updatable);

		Drawable* drawable;
		if (drawable = dynamic_cast<Drawable*>(t))
			drawables.push_back(drawable);

		return t;
	}

	template <class T>
	T* GetComponent()
	{
		T* t;
		for (auto& component : components)
		{
			if (t = dynamic_cast<T*>(component.get()))
				return t;
		}
		return nullptr;
	}

	template <class T>
	bool RemoveComponent()
	{
		T* t;

		for (int i = 0; i < components.size(); ++i)
		{
			if (t = dynamic_cast<T*>(components[i].get()))
			{
				Updatable* updatable;
				if (updatable = dynamic_cast<Updatable*>(t))
					std::erase(updatables, updatable);

				Drawable* drawable;
				if (drawable = dynamic_cast<Drawable*>(t))
					std::erase(drawables, drawable);

				components.erase(components.begin() + i);

				return true;
			}
		}

		return false;
	}

	bool RemoveComponent(Component* toRemove);

	int GetComponentCount();
	Component* GetComponentAt(int at);
	Stage* GetStage();

private:
	Stage* stage;
	GameObject* prev; 
	GameObject* next;

	std::vector<std::unique_ptr<Component>> components;
	std::vector<Updatable*> updatables;
	std::vector<Drawable*> drawables;
};
