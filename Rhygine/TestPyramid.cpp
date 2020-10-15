#include "TestPyramid.h"
#include "RhyBindables.h"
#include "Window.h"
#include "Gfx.h"
#include "TestCamera.h"
#include "GameObject.h"
#include "RotateAround.h"
#include "BasicShader.h"

#include <vector>

void TestPyramid::AddData(GameObject* toAddTo)
{
	Transform* transform = AddTransform(toAddTo);
	Drawer* drawer = AddDrawer(toAddTo);

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

	drawer->AddBindable(std::make_unique<VertBuffer<Vertex>>(verts, 0));
	drawer->AddBindable(std::make_unique<IndexBufferUS>(indexes, 0));
	drawer->AddBindable(std::make_unique<PrimitiveTopolpgy>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	drawer->AddBindable(std::make_unique<BasicShader>(L"BasicPix.hlsl", L"BasicVert.hlsl", &inputLayoutDesc));

	transform->GetGameObject()->AddComponent<RotateAround>()->rotationSpeed = RhyM::Vec3(1.0, 0.0f, 0.0f);
}
