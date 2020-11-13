#include "Component.h"
#include "GameObject.h"
#include "Window.h"
#include "Keys.h"
#include "Mouse.h"
#include "Stage.h"
#include "SpriteBatch.h"

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

void Component::SetGameObject(GameObject* game)
{
	gameObject = game;
}
