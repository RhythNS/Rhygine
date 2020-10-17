#include "Component.h"
#include "GameObject.h"
#include "Window.h"
#include "Keys.h"
#include "Mouse.h"

GameObject* Component::GetGameObject()
{
	return gameObject;
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

void Component::SetGameObject(GameObject* game)
{
	gameObject = game;
}
