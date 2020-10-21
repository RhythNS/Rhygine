#pragma once
#include "Updatable.h"
#include "Drawable.h"

class GameObject;
class Camera;

/// <summary>
/// Manages all gameobjects.
/// </summary>
class Stage : public Updatable, public Drawable
{
public:
	Stage();
	~Stage();
	/// <summary>
	/// Creates a new gameobjects and places it to the end of the gameobject list.
	/// </summary>
	/// <returns>A refernce to the newly gameobject.</returns>
	GameObject* CreateGameObject();
	/// <summary>
	/// Creates a new gameobjects and places it after a specified gameobject inside the list.
	/// </summary>
	/// <returns>A refernce to the newly gameobject.</returns>
	GameObject* CreateGameObjectAfter(GameObject* gameObject);
	/// <summary>
	/// Deletes a gameobject.
	/// </summary>
	/// <param name="gameObject">The gameobject that should be removed.
	/// Deleting the front or back will crash the program.</param>
	void RemoveGameObject(GameObject* gameObject);
	/// <summary>
	/// The first gameobject of the list. Should not be deleted.
	/// </summary>
	GameObject* GetFront();
	/// <summary>
	/// The last gameobject of the list. Should not be deleted.
	/// </summary>
	GameObject* GetBack();

	/// <summary>
	/// Get a reference to the camera.
	/// </summary>
	Camera* GetCamera();

	/// <summary>
	/// Updates all gameobjects.
	/// </summary>
	void Update();
	/// <summary>
	/// Draws all gameobjects.
	/// </summary>
	void Draw();

private:
	GameObject* front;
	GameObject* back;
	Camera* camera;

#define FOR_EACH_GAMEOBJECT( method ) \
	GameObject* current = front; \
	while (current->next != nullptr) \
	{ \
		current->method; \
		current = current->next; \
	} \
	current->method;

	// pool of gameobjects
};
