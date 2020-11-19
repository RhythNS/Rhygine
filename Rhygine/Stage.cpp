#include "Stage.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "UIRootElement.h"
#include "Window.h"

Stage::Stage() : width(Window::GetInstance()->GetWidth()), height(Window::GetInstance()->GetHeight()),
front(new GameObject(this)), back(new GameObject(this)),
camera(front->AddComponent<Camera>()), rootUI(back->AddComponent<UIRootElement>()),
uiCamera(RhyM::Vec3(static_cast<float>(-width) / 2, static_cast<float>(-height) / 2, -5.0f))
{
	// Move the camera a bit back to not start at 0,0,0
	camera->GetTransform()->localPosition = RhyM::Vec3(0.0f, 0.0f, -4.0f);

	front->next = back;
	back->prev = front;
}

Stage::~Stage()
{
	// delete the ui
	rootUI->Delete();

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

GameObject* Stage::CreateGameObject(std::string name)
{
	GameObject* gameObject = CreateGameObjectAfter(back->prev);
	gameObject->name = name;
	return gameObject;
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

GameObject* Stage::CreateGameObjectAfter(GameObject* gameObject, std::string name)
{
	GameObject* newObject = CreateGameObjectAfter(gameObject->prev);
	newObject->name = name;
	return newObject;
}

void Stage::RemoveGameObject(GameObject* gameObject)
{
	assert(gameObject != front && gameObject != back);
	toRemoveGameobjects.push(gameObject);
}

GameObject* Stage::GetFront()
{
	return front;
}

GameObject* Stage::GetBack()
{
	return back;
}

Camera* Stage::Get3DCamera()
{
	return camera;
}

OrthographicCamera* Stage::Get2DCamera()
{
	return &orthoCamera;
}

OrthographicCamera* Stage::GetUICamera()
{
	return &uiCamera;
}

UIRootElement* Stage::GetUIRoot()
{
	return rootUI;
}

SpriteBatch* Stage::GetSpriteBatch()
{
	return &batch;
}

void Stage::Update()
{
	FOR_EACH_GAMEOBJECT(Update());


	while (toRemoveGameobjects.size() != 0)
	{
		GameObject* gameObject = toRemoveGameobjects.front();
		toRemoveGameobjects.pop();

		gameObject->OnRemove();

		// Remove the gameobject from the next and prev.
		// prev -> gameobject -> next
		// will now be
		// prev -> next
		gameObject->prev->next = gameObject->next;
		gameObject->next->prev = gameObject->prev;

		delete gameObject;
	}
}

void Stage::Draw()
{
	camera->UpdateMatrix();
	batch.Begin(&orthoCamera);
	FOR_EACH_GAMEOBJECT(Draw());
	batch.End();

	batch.Begin(&uiCamera);
	rootUI->Draw(&batch);
	batch.End();
}

void Stage::OnResize(int newWidth, int newHeight)
{
	uiCamera.position.m_floats[0] = static_cast<float>(-newWidth / 2);
	uiCamera.position.m_floats[1] = static_cast<float>(-newHeight / 2);

	rootUI->SetSize(static_cast<float>(newWidth), static_cast<float>(newHeight));
	rootUI->SetPos(0.0f, 0.0f);

	width = newWidth;
	height = newHeight;
}

void Stage::OnMouseMove(int x, int y)
{
	RhyM::Vec2 trans = RhyM::Vec2(
		x + uiCamera.position.m_floats[0] + width / 2,
		height - y + uiCamera.position.m_floats[1] + height / 2
	);

	rootUI->OnMouseMove(trans);
}

void Stage::OnMouseDown()
{
	rootUI->ClickDown();
}

void Stage::OnMouseUp()
{
	rootUI->ClickUp();
}
