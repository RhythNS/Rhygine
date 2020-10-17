#include "Stage.h"
#include "GameObject.h"
#include "Camera.h"

Stage::Stage() : front(new GameObject()), back(new GameObject()), camera(front->AddComponent<Camera>())
{
	front->next = back;
	back->prev = front;
}

Stage::~Stage()
{
	GameObject* current = front->next;
	while (current->next != nullptr) 
	{
		delete current->prev;
		current = current->next;
	}
	delete current;
}

GameObject* Stage::CreateGameObject()
{
	return CreateGameObjectAfter(back->prev);
}

GameObject* Stage::CreateGameObjectAfter(GameObject* creteAfter)
{
	GameObject* gameObject = new GameObject();

	gameObject->stage = this;

	gameObject->prev = creteAfter;
	gameObject->next = creteAfter->next;
	creteAfter->next = gameObject;
	gameObject->next->prev = gameObject;

	gameObject->Init();
	return gameObject;
}

void Stage::RemoveGameObject(GameObject* gameObject)
{
	gameObject->prev->next = gameObject->next;
	gameObject->next->prev = gameObject->prev;

	delete gameObject;
}

GameObject* Stage::GetFront()
{
	return front;
}

GameObject* Stage::GetBack()
{
	return back;
}

Camera* Stage::GetCamera()
{
	return camera;
}

void Stage::Update()
{
	FOR_EACH_GAMEOBJECT(Update())
}

void Stage::Draw()
{
	camera->UpdateMatrix();
	FOR_EACH_GAMEOBJECT(Draw())
}
