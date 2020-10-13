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

class Drawer : public Component, public Drawable
{
public:
	void Init();
	void Draw();

	void AddBindable(std::unique_ptr<Bindable> bindable);
	Transform* GetTransform();

private:
	std::vector<std::unique_ptr<Bindable>> bindables;
	std::vector<Updatable*> updatables;
	Transform* transform;
	IndexBufferAmount* indexAmount;
};

