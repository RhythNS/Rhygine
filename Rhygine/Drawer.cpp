#include "Drawer.h"
#include "Bindable.h"
#include "Gfx.h"
#include "GameObject.h"
#include "Transform.h"
#include "Stage.h"
#include "PrimitiveTopolpgy.h"
#include "UnBindable.h"
#include "DrawerUpdatable.h"

void Drawer::Init()
{
	transform = GetGameObject()->GetComponent<Transform>();
	assert(transform);
}

void Drawer::Draw()
{
	// First update all bindables that can update, and then bind them.
	for (auto& updatable : updatables)
	{
		updatable->Update();
	}
	for (auto& drawerUpdatable : drawerUpdatables)
	{
		drawerUpdatable->Update(this);
	}
	for (auto& bindable : bindables)
	{
		bindable->Bind();
	}

	// All bindables are bound, execute the draw call.
	switch (drawMode)
	{
	case DrawMode::List:
		Gfx::GetInstance()->Draw(vertAmount->GetSize());
		break;
	case DrawMode::Indexed:
		Gfx::GetInstance()->DrawIndexed(indexAmount->GetSize());
		break;
	default:
		throw RHY_EXCEP("Unsupported draw mode");
	}

	// Unbind all bindables that inherit from unbindable.
	for (auto& unBindable : unBindables)
	{
		unBindable->UnBind();
	}
}

Bindable* Drawer::AddBindable(std::unique_ptr<Bindable> bindable)
{
	assert(bindable);
	// If it is an updatable, add it to the update list.
	Updatable* updatable;
	if (updatable = dynamic_cast<Updatable*>(bindable.get()))
		updatables.push_back(updatable);

	// If it is an unbindable, add it to the unbindable list.
	UnBindable* unbindable;
	if (unbindable = dynamic_cast<UnBindable*>(bindable.get()))
		unBindables.push_back(unbindable);

	AnalyseBindable(bindable.get());

	// add it to the bindable list
	bindables.push_back(std::move(bindable));

	return bindables[bindables.size() - 1].get();
}

inline void Drawer::AnalyseBindable(Bindable* bindable)
{
	// If it is an index buffer, then set the indexAmount reference to it.
	// This is because, the DrawIndexed() function needs the amount of indicies.
	// Otherwise check if it is a vertex buffer. This is if we need to draw something
	// that is not indexed, since we then need the vertex amount.
	IndexBufferAmount* indAmount;
	if (indAmount = dynamic_cast<IndexBufferAmount*>(bindable))
	{
		indexAmount = indAmount;
		return;
	}
	VertBufferAmount* vertexAmount;
	if (vertexAmount = dynamic_cast<VertBufferAmount*>(bindable))
	{
		vertAmount = vertexAmount;
		return;
	}

	PrimitiveTopology* primitiveTopology;
	if (primitiveTopology = dynamic_cast<PrimitiveTopology*>(bindable))
	{
		switch (*primitiveTopology->GetTopology())
		{
		case D3D_PRIMITIVE_TOPOLOGY_LINELIST:
			drawMode = DrawMode::List;
			break;
		case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
			drawMode = DrawMode::Indexed;
			break;
		default:
			OutputDebugString("Unkown primitive topology. Leaving draw mode in indexed: " + *primitiveTopology->GetTopology());
			break;
		}
		return;
	}

	DrawerUpdatable* drawerUpdatable;
	if (drawerUpdatable = dynamic_cast<DrawerUpdatable*>(bindable))
	{
		drawerUpdatables.push_back(drawerUpdatable);
		return;
	}
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
			
			// if it is an unbindable, remove it from the unbindable list.
			UnBindable* unbindable;
			if (unbindable = dynamic_cast<UnBindable*>(bindable))
				std::erase(unBindables, unbindable);

			DrawerUpdatable* drawerUpdatable;
			if (drawerUpdatable = dynamic_cast<DrawerUpdatable*>(bindable))
				std::erase(drawerUpdatables, drawerUpdatable);
			
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

Camera* Drawer::Get3DCamera()
{
	return GetGameObject()->GetStage()->Get3DCamera();
}
