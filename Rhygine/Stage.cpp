#include "Stage.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"

Stage::Stage() : front(new GameObject(this)), back(new GameObject(this)), camera(front->AddComponent<Camera>())
{
	// Move the camera a bit back to not start at 0,0,0
	camera->GetTransform()->localPosition = RhyM::Vec3(0.0f, 0.0f, -4.0f);

	front->next = back;
	back->prev = front;
}

Stage::~Stage()
{
	// Go through all Gameobjects and manually delete them.
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

GameObject* Stage::CreateGameObjectAfter(GameObject* createAfter)
{
	assert(createAfter != back);
	// Create the gameobject. And set the stage.
	GameObject* gameObject = new GameObject(this);
	gameObject->stage = this;

	// Put the gameobject in between the createAfter and the next of createAfter.
	// ... -> CreateAfter -> next
	// wil now be
	// ... -> CreateAfter -> gameObject -> next
	gameObject->prev = createAfter;
	gameObject->next = createAfter->next;
	createAfter->next = gameObject;
	gameObject->next->prev = gameObject;

	// Lastly init and return a pointer to it.
	gameObject->Init();
	return gameObject;
}

void Stage::RemoveGameObject(GameObject* gameObject)
{
	assert(gameObject != front && gameObject != back);
	// Remove the gameobject from the next and prev.
	// prev -> gameobject -> next
	// will now be
	// prev -> next
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
