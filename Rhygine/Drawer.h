#pragma once
#include "Component.h"
#include "Drawable.h"
#include "Updatable.h"
#include "IndexBuffer.h"

#include <vector>
#include <memory>
#include <DirectXMath.h>

class Bindable;
class Transform;
class Camera;

class Drawer : public Component, public Drawable
{
public:
	void Init();
	void Draw();

	Bindable* AddBindable(std::unique_ptr<Bindable> bindable);
	bool RemoveBindable(Bindable* bindable);

	template <class T>
	T* GetBindable()
	{
		T* t;
		for (auto& bindable : bindables)
		{
			if (t = dynamic_cast<T*>(bindable.get()))
				return t;
		}
		return nullptr;
	}

	Transform* GetTransform();
	Camera* GetCamera();

private:
	std::vector<std::unique_ptr<Bindable>> bindables;
	std::vector<Updatable*> updatables;
	Transform* transform;
	IndexBufferAmount* indexAmount;
};
