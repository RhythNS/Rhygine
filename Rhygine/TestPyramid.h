#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>

#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class Bindable;

class TestPyramid
{
	struct MatrixConstantBuffer {
		DirectX::XMMATRIX transform;
	};

public:
	TestPyramid();
	void Update();
	void Draw();
private:
	std::vector<std::unique_ptr<Bindable>> bindables;
	IndexBufferUS* indexBuffer;
	ConstantVS<MatrixConstantBuffer>* consBuffer;
};

