#pragma once
#include <DirectXMath.h>

#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class Gameobject
{
public:
	struct Vector {
		float x, y, z;
	};

	struct MatrixConstantBuffer {
		DirectX::XMMATRIX transform;
	};

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	Vector position = { 0.0f,0.0f,0.0f };
	Vector rotation = { 0.0f,0.0f,0.0f };
	Vector scale = { 1.0f,1.0f,1.0f };
protected:
	std::vector<std::unique_ptr<Bindable>> bindables;
	IndexBufferUS* indexBuffer;
	ConstantVS<MatrixConstantBuffer>* consBuffer;

	MatrixConstantBuffer GetCurrentTransform();
};

