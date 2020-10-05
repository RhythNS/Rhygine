#pragma once
#include <DirectXMath.h>

#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"

class Gameobject
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	Transform transform;
protected:
	void CreateTransform();

	std::vector<std::unique_ptr<Bindable>> bindables;
	IndexBufferUS* indexBuffer;
	ConstantVS<Transform::TransformBuffer>* consBuffer;
};

