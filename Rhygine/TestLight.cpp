#include "TestLight.h"
#include "RhyBindables.h"
#include "Rhyimgui.h"
#include "Window.h"
#include "RhyWin.h"
#include "TestLightComponent.h"
#include "Transform.h"
#include "Drawer.h"
#include "BasicShader.h"

#include <cmath>

void TestLight::AddData(GameObject* toAddTo)
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
		{-0.5f, -0.5f, -0.5f, 255,0,0,0},
		{ 0.5f, -0.5f, -0.5f, 255,0,0,0},
		{-0.5f,  0.5f, -0.5f, 255,0,0,0},
		{ 0.5f,  0.5f, -0.5f, 255,0,0,0},
		{-0.5f, -0.5f,  0.5f, 255,0,0,0},
		{ 0.5f, -0.5f,  0.5f, 255,0,0,0},
		{-0.5f,  0.5f,  0.5f, 255,0,0,0},
		{ 0.5f,  0.5f,  0.5f, 255,0,0,0}
	};

	std::vector<unsigned short> indexes{
		0,2,3, 0,3,1,
		4,6,2, 4,2,0,
		5,7,6, 5,6,4,
		1,3,7, 1,7,5,
		2,6,3, 6,7,3,
		0,1,4, 1,5,4,
	};

	drawer->AddBindable(std::make_unique<VertBuffer<Vertex>>(verts, 0));
	drawer->AddBindable(std::make_unique<IndexBufferUS>(indexes, 0));
	drawer->AddBindable(std::make_unique<PrimitiveTopolpgy>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	drawer->AddBindable(std::make_unique<BasicShader>(L"BasicPix.hlsl", L"BasicVert.hlsl", &inputLayoutDesc));

	toAddTo->AddComponent<TestLightComponent>();
}
