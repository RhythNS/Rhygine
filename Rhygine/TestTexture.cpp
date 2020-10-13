#include "TestTexture.h"
#include "Window.h"
#include "VertBuffer.h"
#include "PixShader.h"
#include "VertShader.h"
#include "InputLayout.h"
#include "PrimitiveTopolpgy.h"
#include "Sampler.h"

#include <string>

void TestTexture::Init()
{
	transform.position = { 10.0f, 0.0f, 10.0f };

	struct Vertex {
		struct {
			float x, y, z;
		} pos;
		struct {
			float u;
			float v;
		} texCoords;
	};

	std::vector<Vertex> verts =
	{
		{-1.0f, -1.0f,  0.0f, 0.0f, 1.0f},
		{-1.0f,  1.0f,  0.0f, 0.0f, 0.0f},
		{ 1.0f,  1.0f,  0.0f, 1.0f, 0.0f},
		{ 1.0f, -1.0f,  0.0f, 1.0f, 1.0f}
	};

	std::vector<unsigned short> indexes =
	{
		0, 1, 2,
		3, 0, 2
	};

	bindables.push_back(std::make_unique<VertBuffer<Vertex>>(verts, 0));

	bindables.push_back(std::make_unique<IndexBufferUS>(indexes, 0));
	indexBuffer = static_cast<IndexBufferUS*>(bindables[bindables.size() - 1].get());

	bindables.push_back(std::make_unique<PrimitiveTopolpgy>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	CreateTransform();

	for (int i = 1; i < 21; i++)
	{
		textures.push_back(std::make_unique<Texture>(("TestModels\\Sprite\\Walk (" + std::to_string(i) + ").png").c_str(), 0));
	}

	bindables.push_back(std::make_unique<Sampler>(0));

	bindables.push_back(std::make_unique<PixShader>(L"TexPix.cso"));
	bindables.push_back(std::make_unique<VertShader>(L"TexVert.cso"));
	ID3DBlob* blob = (static_cast<VertShader*>(bindables[bindables.size() - 1].get()))->GetBlob();

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "texCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	bindables.push_back(std::make_unique<InputLayout>(inputLayoutDesc, blob));
}

void TestTexture::Update()
{
	timer -= Window::GetInstance()->time.GetDelta();
	if (timer <= 0)
	{
		timer = timeToChange;
		if (++atTexture == textures.size())
			atTexture = 0;
	}
	GameObject::Update();
}

void TestTexture::Draw()
{
	textures[atTexture]->Bind();
	GameObject::Draw();
}

