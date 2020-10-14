#include "Drawer.h"
#include "Bindable.h"
#include "Gfx.h"
#include "GameObject.h"

void Drawer::Init()
{
	transform = GetGameObject()->GetComponent<Transform>();
}

void Drawer::Draw()
{
	for (auto& updatable : updatables)
	{
		updatable->Update();
	}
	for (auto& bindable : bindables)
	{
		bindable->Bind();
	}

	Gfx::GetInstance()->DrawIndexed(indexAmount->GetSize());
}

Bindable* Drawer::AddBindable(std::shared_ptr<Bindable> bindable)
{
	Updatable* updatable;
	if (updatable = dynamic_cast<Updatable*>(bindable.get()))
		updatables.push_back(updatable);

	IndexBufferAmount* indAmount;
	if (indAmount = dynamic_cast<IndexBufferAmount*>(bindable.get()))
		indexAmount = indAmount;

	bindable->drawer = this;

	bindables.push_back(bindable);

	return bindable.get();
}

bool Drawer::RemoveBindable(Bindable* bindable)
{
	for (auto bind = std::begin(bindables); bind != std::end(bindables); ++bind)
	{
		if ((*bind).get() == bindable)
		{
			bindables.erase(bind);

			Updatable* updatable;
			if (updatable = dynamic_cast<Updatable*>(bindable))
				std::erase(updatables, updatable);

			return true;
		}
	}

	return false;
}

Transform* Drawer::GetTransform()
{
	return transform;
}
