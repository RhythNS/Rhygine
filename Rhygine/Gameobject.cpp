#include "GameObject.h"
#include "Updatable.h"
#include "Drawable.h"

#include <memory>

void GameObject::Init()
{
}

void GameObject::Update()
{
	for (auto& updatable : updatables)
	{
		updatable->Update();
	}
}

void GameObject::Draw()
{
	for (auto& drawable : drawables)
	{
		drawable->Draw();
	}
}

