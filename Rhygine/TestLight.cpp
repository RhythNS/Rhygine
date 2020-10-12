#include "TestLight.h"
#include "RhyBindables.h"
#include "Rhyimgui.h"
#include "Window.h"
#include "RhyWin.h"
#include <cmath>

TestLight::TestLight(int id, float* dir) : id(id)
{
	direction.x = dir[0];
	direction.y = dir[1];
	direction.z = dir[2];
	transform.rotation = Quat(dir[0], dir[1], dir[2]);
}

void TestLight::Init()
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

	bindables.push_back(std::make_unique<VertBuffer<Vertex>>(verts, 0));

	bindables.push_back(std::make_unique<IndexBufferUS>(indexes, 0));
	indexBuffer = static_cast<IndexBufferUS*>(bindables[bindables.size() - 1].get());

	bindables.push_back(std::make_unique<PrimitiveTopolpgy>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	CreateTransform();

	bindables.push_back(std::make_unique<PixShader>(L"BasicPix.hlsl"));
	bindables.push_back(std::make_unique<VertShader>(L"BasicVert.hlsl"));
	ID3DBlob* blob = (static_cast<VertShader*>(bindables[bindables.size() - 1].get()))->GetBlob();

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	bindables.push_back(std::make_unique<InputLayout>(inputLayoutDesc, blob));
}

void TestLight::Update()
{
	ImGui::Begin(("Light " + id), &guiWindowOpen);
	ImGui::DragFloat3("Position", &transform.position.x, 0.1f, -10.0f, 10.0f, "%.3f", 0);
	if (ImGui::DragFloat3("Rotation", &direction.x, 0.01f, -1.0f, 1.0f, "%.3f", 0)) {
		transform.rotation = Quat(direction.x * rhyM::PI, direction.y * rhyM::PI, direction.z * rhyM::PI);
	}
	ImGui::End();

	Gameobject::Update();
}
