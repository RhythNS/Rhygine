#include "Component.h"
#include "GameObject.h"

GameObject* Component::GetGameObject()
{
	return gameObject;
}

void Component::SetGameObject(GameObject* game)
{
	gameObject = game;
}
