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
	/// Enables an component to recieve update and draw calls.
	/// </summary>
	/// <param name="component">The component that should enabled.</param>
	void EnableComponent(Component* component);
	/// <summary>
	/// Disables an component. It no longer recieves update and draw calls.
	/// </summary>
	/// <param name="component">The component to be disabled.</param>
	void DisableComponent(Component* component);

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
			// is this the type of component?
			if (t = dynamic_cast<T*>(components[i].get()))
			{
				// If it is updatable, remove it from the updatables.
				Updatable* updatable;
				if (updatable = dynamic_cast<Updatable*>(t))
					std::erase(updatables, updatable);

				// If it is drawable, remove it from the drawables.
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

	/// <summary>
	/// Looks through a vector of unique_ptr of specified type and returns true
	/// if a given instance is found inside the vector.
	/// </summary>
	template <class T>
	bool IsInUniqueVector(std::vector<std::unique_ptr<T>>* vec, T* t)
	{
		for (auto& search : *vec)
		{
			if (search.get() == t)
			{
				return true;
			}
		}
		return false;
	}

	/// <summary>
	/// Looks through a vector of pointers of specified type and returns true
	/// if a given instance is found inside the vector.
	/// </summary>
	template <class T>
	bool IsInVector(std::vector<T*>* vec, T* t)
	{
		for (auto& search : *vec)
		{
			if (search == t)
			{
				return true;
			}
		}
		return false;
	}

	std::vector<std::unique_ptr<Component>> components;
	std::vector<Updatable*> updatables;
	std::vector<Drawable*> drawables;
};
