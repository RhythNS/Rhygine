#include "Drawer.h"
#include "Bindable.h"
#include "Gfx.h"
#include "GameObject.h"
#include "Transform.h"
#include "Stage.h"

void Drawer::Init()
{
	transform = GetGameObject()->GetComponent<Transform>();
}

void Drawer::Draw()
{
	// First update all bindables that can update, and then bind them.
	for (auto& updatable : updatables)
	{
		updatable->Update();
	}
	for (auto& bindable : bindables)
	{
		bindable->Bind();
	}

	// All bindables are bound, execute the draw call.
	Gfx::GetInstance()->DrawIndexed(indexAmount->GetSize());
}

Bindable* Drawer::AddBindable(std::unique_ptr<Bindable> bindable)
{
	// If it is an updatable, add it to the update list.
	Updatable* updatable;
	if (updatable = dynamic_cast<Updatable*>(bindable.get()))
		updatables.push_back(updatable);

	// If it is an index buffer, then set the indexAmount reference to it.
	// This is because, the DrawIndexed() function needs the amount of indicies.
	IndexBufferAmount* indAmount;
	if (indAmount = dynamic_cast<IndexBufferAmount*>(bindable.get()))
		indexAmount = indAmount;

	// Init the bindable
	bindable->drawer = this;
	bindable->Init();

	// add it to the bindable list
	bindables.push_back(std::move(bindable));

	return bindables[bindables.size() - 1].get();
}

bool Drawer::RemoveBindable(Bindable* bindable)
{
	// Go through all bindables and check if it is in the list
	for (auto bind = std::begin(bindables); bind != std::end(bindables); ++bind)
	{
		if ((*bind).get() == bindable)
		{
			// if it is an updatable, remove it from the updatable list.
			Updatable* updatable;
			if (updatable = dynamic_cast<Updatable*>(bindable))
				std::erase(updatables, updatable);

			bindables.erase(bind);
		
			return true;
		}
	}
	// bindable was not found.
	return false;
}

Transform* Drawer::GetTransform()
{
	return transform;
}

Camera* Drawer::GetCamera()
{
	return GetGameObject()->GetStage()->GetCamera();
}
