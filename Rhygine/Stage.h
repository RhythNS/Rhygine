#pragma once
#include "Updatable.h"
#include "Drawable.h"

class GameObject;
class Camera;

class Stage : public Updatable, public Drawable
{
public:
	Stage();
	~Stage();
	GameObject* CreateGameObject();
	GameObject* CreateGameObjectAfter(GameObject* gameObject);
	void RemoveGameObject(GameObject* gameObject);
	GameObject* GetFront();
	GameObject* GetBack();

	Camera* GetCamera();

	void Update();
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
