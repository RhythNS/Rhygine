#include "TestPyramid.h"
#include "VertBuffer.h"
#include "PixShader.h"
#include "VertShader.h"
#include "InputLayout.h"
#include "PrimitiveTopolpgy.h"
#include "Window.h"
#include "Gfx.h"
#include "TestCamera.h"

#include <vector>

TestPyramid::TestPyramid(Vector pos)
{
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
}

void TestPyramid::Init()
{
	struct Vertex {
		struct {
			float x, y, z;
		} pos;
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};

	std::vector<Vertex> verts = {
		{-0.5f, -0.5f, 0.5f, 255,0,0,0},
		{0.5f, -0.5f, 0.5f, 0,255,0,0},
		{0.5f, -0.5f, -0.5f, 0,0,255,0},
		{-0.5f, -0.5f, -0.5f, 255,0,0},
		{0.0f, 0.5f, 0.0f, 0,255,0,0},
	};

	std::vector<unsigned short> indexes{
		3,4,2, 2,4,1,
		1,4,0, 0,4,3,
		2,1,0, 0,3,2
	};

	bindables.push_back(std::make_unique<VertBuffer<Vertex>>(verts, 0));

	bindables.push_back(std::make_unique<IndexBufferUS>(indexes, 0));
	indexBuffer = static_cast<IndexBufferUS*>(bindables[bindables.size() - 1].get());

	bindables.push_back(std::make_unique<PrimitiveTopolpgy>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


	MatrixConstantBuffer matConsBuffer = GetCurrentTransform();
	bindables.push_back(std::make_unique<ConstantVS<MatrixConstantBuffer>>(matConsBuffer, 0));
	consBuffer = static_cast<ConstantVS<MatrixConstantBuffer>*>(bindables[bindables.size() - 1].get());


	bindables.push_back(std::make_unique<PixShader>(L"BasicPix.cso"));
	bindables.push_back(std::make_unique<VertShader>(L"BasicVert.cso"));
	ID3DBlob* blob = (static_cast<VertShader*>(bindables[bindables.size() - 1].get()))->GetBlob();

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	bindables.push_back(std::make_unique<InputLayout>(inputLayoutDesc, blob));
}

void TestPyramid::Update()
{
	rotation.y += Window::GetInstance()->time.GetDelta();
	Gameobject::Update();
}
