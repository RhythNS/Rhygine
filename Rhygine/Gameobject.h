#pragma once
#include "Updatable.h"
#include "Drawable.h"
#include "Component.h"

#include <vector>
#include <memory>

class GameObject
{
public:
	void Init();
	void Update();
	void Draw();

	template <class T>
	T* AddComponent()
	{
		components.push_back(std::make_unique<T>());
		T* t = static_cast<T*>(bindables[bindables.size() - 1].get());
		t.SetGameObject(this);
		t.Init();

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
			if (t = dynamic_cast<T*>(t))
				return t;
		}
		return nullptr;
	}

	template <class T>
	bool RemoveComponent()
	{
		T* t;

		for (auto& component : components)
		{
			if (t = dynamic_cast<T*>(t)) 
			{
				components.erase(component);

				Updatable* updatable;
				if (updatable = dynamic_cast<Updatable*>(t))
					std::erase(updatables, updatable);

				Drawable* drawable;
				if (drawable = dynamic_cast<Drawable*>(t))
					std::erase(drawables, drawable);

				return true;
			}
		}

		return false;
	}
private:
	std::vector<std::unique_ptr<Component>> components;
	std::vector<Updatable*> updatables;
	std::vector<Drawable*> drawables;
};
