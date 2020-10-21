#pragma once
#include "Updatable.h"
#include "Drawable.h"
#include "Component.h"

#include <vector>
#include <memory>

class Stage;

/// <summary>
/// Represent an object in the simulation. Gameobjects are being held by the Stage. The
/// gameobject holds components which are used for storage, for updating or for drawing.
/// </summary>
class GameObject
{
	friend class Stage;
public:
	/// <summary>
	/// Called when after the stage was assigned and initializes the gameobject.
	/// </summary>
	void Init();
	/// <summary>
	/// Called every frame. This is used to update the gameobject for the simulation.
	/// </summary>
	void Update();
	/// <summary>
	/// Called ever frame. This is used to draw the gameobject onto the screen.
	/// </summary>
	void Draw();

	/// <summary>
	/// Adds a component to the gameobject.
	/// </summary>
	/// <typeparam name="T">The type of component.</typeparam>
	/// <returns>A reference to the component.</returns>
	template <class T>
	T* AddComponent()
	{
		components.push_back(std::make_unique<T>());
		T* t = static_cast<T*>(components[components.size() - 1].get());
		t->SetGameObject(this);
		t->Init();

		// Check if the component is an updatable or an drawable
		// and add them to the drawables or updatables lists.
		Updatable* updatable;
		if (updatable = dynamic_cast<Updatable*>(t))
			updatables.push_back(updatable);

		Drawable* drawable;
		if (drawable = dynamic_cast<Drawable*>(t))
			drawables.push_back(drawable);

		return t;
	}

	/// <summary>
	/// Gets the first component of the specified type.
	/// </summary>
	/// <typeparam name="T">The type of the component.</typeparam>
	/// <returns>A reference to the component or nullptr when no such component was found.</returns>
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

	/// <summary>
	/// Removes the first component of the specified type.
	/// </summary>
	/// <typeparam name="T">The type of the component.</typeparam>
	/// <returns>Wheter a component was found and deleted or not.</returns>
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

	/// <summary>
	/// Removes a component based on a reference to the component.
	/// </summary>
	/// <param name="toRemove">The component to be removed.</param>
	/// <returns>Wheter the component could be removed or not.</returns>
	bool RemoveComponent(Component* toRemove);

	/// <summary>
	/// Gets the amount of components that are currently on the gameobject.
	/// </summary>
	int GetComponentCount();
	/// <summary>
	/// Gets the component at a specific index.
	/// </summary>
	/// <param name="at">The index for the component.</param>
	/// <returns>A reference to the component.</returns>
	Component* GetComponentAt(int at);
	/// <summary>
	/// Gets a reference to the stage that this gameobject is on.
	/// </summary>
	Stage* GetStage();

private:
	Stage* stage;
	/// <summary>
	/// The previous gameobject of the linked list.
	/// </summary>
	GameObject* prev;
	/// <summary>
	/// The next gameobject of the linked list.
	/// </summary>
	GameObject* next;

	std::vector<std::unique_ptr<Component>> components;
	std::vector<Updatable*> updatables;
	std::vector<Drawable*> drawables;
};
