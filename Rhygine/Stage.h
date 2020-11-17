#pragma once
#include "Updatable.h"
#include "Drawable.h"
#include "SpriteBatch.h"
#include "OrthographicCamera.h"

#include <string>
#include <queue>

class GameObject;
class Camera;
class Scene;
class UIRootElement;
/// <summary>
/// Manages all gameobjects.
/// </summary>
class Stage : public Updatable, public Drawable
{
	friend class Scene;
public:
	Stage();
	~Stage();
	/// <summary>
	/// Creates a new gameobjects and places it to the end of the gameobject list.
	/// </summary>
	/// <returns>A refernce to the newly gameobject.</returns>
	GameObject* CreateGameObject();
	/// <summary>
	/// Creates a new gameobjects and places it to the end of the gameobject list.
	/// </summary>
	/// <param name="name">The name of the gameobject.</param>
	/// <returns>A refernce to the newly gameobject.</returns>
	GameObject* CreateGameObject(std::string name);
	/// <summary>
	/// Creates a new gameobjects and places it after a specified gameobject inside the list.
	/// </summary>
	/// <returns>A refernce to the newly gameobject.</returns>
	GameObject* CreateGameObjectAfter(GameObject* gameObject);
	/// <summary>
	/// Creates a new gameobjects and places it after a specified gameobject inside the list.
	/// </summary>
	/// <param name="name">The name of the gameobject.</param>
	/// <returns>A refernce to the newly gameobject.</returns>
	GameObject* CreateGameObjectAfter(GameObject* gameObject, std::string name);
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
	/// Get a reference to the 3D camera.
	/// </summary>
	Camera* Get3DCamera();
	/// <summary>
	/// Gets a reference to the 2D camera.
	/// </summary>
	OrthographicCamera* Get2DCamera();
	
	/// <summary>
	/// Gets a reference to the 2D camera.
	/// </summary>
	OrthographicCamera* GetUICamera();

	/// <summary>
	/// Gets a reference to the root element of the ui.
	/// </summary>
	UIRootElement* GetUIRoot();

	/// <summary>
	/// Gets a reference to the spritebatch.
	/// </summary>
	SpriteBatch* GetSpriteBatch();

	/// <summary>
	/// Updates all gameobjects.
	/// </summary>
	void Update();
	/// <summary>
	/// Draws all gameobjects.
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// Called when the window resizes.
	/// </summary>
	/// <param name="newWidth">The new width of the window.</param>
	/// <param name="newHeight">The new height of the window.</param>
	void OnResize(int newWidth, int newHeight);

	/// <summary>
	/// Called when the mouse moves.
	/// </summary>
	/// <param name="x">The new x coordinate of the mouse.</param>
	/// <param name="y">The new y coordinate of the mouse.</param>
	void OnMouseMove(int x, int y);

	/// <summary>
	/// Called when the left mouse button was pressed.
	/// </summary>
	void OnMouseDown();

	/// <summary>
	/// Called when the left mouse button was released.
	/// </summary>
	void OnMouseUp();

	int width, height;
	GameObject* front;
	GameObject* back;
	Camera* camera;
	OrthographicCamera orthoCamera, uiCamera;
	SpriteBatch batch;
	UIRootElement* rootUI;
	std::queue<GameObject*> toRemoveGameobjects;

	// Iterates over each gameobject that is inside the stage, and executes
	// a given method.
#define FOR_EACH_GAMEOBJECT( method ) \
	GameObject* current = front; \
	while (current->next != nullptr) \
	{ \
		current->method; \
		current = current->next; \
	} \
	current->method
};
