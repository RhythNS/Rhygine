#include "Component.h"
#include "GameObject.h"
#include "Window.h"
#include "Keys.h"
#include "Mouse.h"
#include "Stage.h"
#include "SpriteBatch.h"
#include "TaskManager.h"
#include "Coroutine.h"

GameObject* Component::GetGameObject()
{
	return gameObject;
}

bool Component::IsEnabled()
{
	return enabled;
}

void Component::Enable()
{
	if (!enabled)
		gameObject->EnableComponent(this);
}

void Component::Disable()
{
	if (enabled)
		gameObject->DisableComponent(this);
}

float Component::GetDelta()
{
	return Window::GetInstance()->time.GetDelta();
}

Mouse* Component::GetMouse()
{
	return &Window::GetInstance()->mouse;
}

Keys* Component::GetKeys()
{
	return &Window::GetInstance()->keys;
}

SpriteBatch* Component::GetSpriteBatch()
{
	return gameObject->GetStage()->GetSpriteBatch();
}

void Component::AddOneShot(ParallelUpdatable* parallel)
{
	Window::GetInstance()->GetTaskManager()->AddOneShot(parallel);
}

std::weak_ptr<Coroutine> Component::StartCoroutine(Yielder yielder, std::function<void()> onFinish)
{
	return gameObject->StartCoroutine(yielder, onFinish);
}

void Component::SetGameObject(GameObject* game)
{
	gameObject = game;
}
