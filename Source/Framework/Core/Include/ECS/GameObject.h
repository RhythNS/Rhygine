#pragma once
#include <ECS/Updatable.h>
#include <ECS/LateUpdatable.h>
#include <ECS/Drawable.h>
#include <ECS/Component.h>
#include <ECS/ParallelUpdatable.h>
#include <ECS/Coroutine/Coroutine.h>

#include <vector>
#include <memory>
#include <string>

class Stage;
class TaskManager;

/// <summary>
/// Represent an object in the simulation. Gameobjects are being held by the Stage. The
/// gameobject holds components which are used for storage, for updating or for drawing.
/// </summary>
class GameObject
{
	friend class Stage;
	friend class TaskManager;
	friend class Coroutine;
public:
	GameObject() = delete;

	/// <summary>
	/// Called when after the stage was assigned and initializes the gameobject.
	/// </summary>
	void Init();
	/// <summary>
	/// Called every frame. This is used to update the gameobject for the simulation.
	/// </summary>
	void Update();
	/// <summary>
	/// Called after update on every component that implements LateUpdatable.
	/// </summary>
	void LateUpdate();
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

		Component* comp = dynamic_cast<Component*>(t);
		comp->SetGameObject(this);
		comp->Init();
		comp->OnEnabled();

		// Check if the component is an updatable or an drawable
		// and add them to the drawables or updatables lists.
		Updatable* updatable;
		if (updatable = dynamic_cast<Updatable*>(t))
			updatables.push_back(updatable);

		Drawable* drawable;
		if (drawable = dynamic_cast<Drawable*>(t))
			drawables.push_back(drawable);

		LateUpdatable* late;
		if (late = dynamic_cast<LateUpdatable*>(t))
			lateUpdatables.push_back(late);

		ParallelUpdatable* parallel;
		if (parallel = dynamic_cast<ParallelUpdatable*>(t))
			parallels.push_back(parallel);

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

				// If it is lateUpdatable, remove it from the late updatables.
				LateUpdatable* late;
				if (late = dynamic_cast<LateUpdatable*>(t))
					std::erase(lateUpdatables, late);

				// If it is a Parallel, remove it from parallels.
				ParallelUpdatable* parallel;
				if (parallel = dynamic_cast<ParallelUpdatable*>(t))
					std::erase(parallels, parallel);

				Component* comp = dynamic_cast<Component*>(t);
				comp->OnDisabled();
				comp->OnRemove();

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
	/// Called when the gameobject is about to be removed.
	/// </summary>
	void OnRemove();

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
	/// Starts a coroutine on this GameObject.
	/// </summary>
	/// <param name="yielder">The yielder method that this coroutine uses.</param>
	/// <param name="onFinish">The function that is called when the coroutine finishes.</param>
	/// <returns>A weak reference to the coroutine.</returns>
	std::weak_ptr<Coroutine> StartCoroutine(Yielder yielder, std::function<void()> onFinish = nullptr);

	/// <summary>
	/// Gets a reference to the stage that this gameobject is on.
	/// </summary>
	Stage* GetStage();

	/// <summary>
	/// Wheter this gameobject survives a scene change.
	/// </summary>
	bool survivesSceneChange = false;

	std::string name;

private:
	GameObject(Stage* stage);

	/// <summary>
	/// Removes the coroutine from this GameObject. Should only be called from inside the coroutine.
	/// </summary>
	/// <param name="coroutine">The coroutine to be removed.</param>
	void RemoveCoroutine(Coroutine* coroutine);

	Stage* stage;
	/// <summary>
	/// The previous gameobject of the linked list.
	/// </summary>
	GameObject* prev = nullptr;
	/// <summary>
	/// The next gameobject of the linked list.
	/// </summary>
	GameObject* next = nullptr;

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
	std::vector<LateUpdatable*> lateUpdatables;
	std::vector<Drawable*> drawables;
	std::vector<ParallelUpdatable*> parallels;
	std::vector<std::shared_ptr<Coroutine>> coroutines;
};
