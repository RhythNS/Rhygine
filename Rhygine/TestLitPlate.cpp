#include "TestLitPlate.h"

#include "RhyBindables.h"
#include "Rhyimgui.h"
#include "TestLight.h"

TestLitPlate::TestLitPlate(std::array<TestLight*, 10> lights) : lights(lights), guiWindowOpen(true), pixBuffer(nullptr)
{
}

void TestLitPlate::Init()
{
	transform.position.Set(0.0f, 0.0f, 5.0f);

	struct Vertex {
		struct {
			float x, y, z;
		} pos;
		struct {
			float u;
			float v;
		} texCoords;
		struct {
			float nx, ny, nz;
		} normal;
	};

	std::vector<Vertex> verts =
	{
		{-1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f},
		{-1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f},
		{ 1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f},
		{ 1.0f, -1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f}
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

	UpdateBuffer();
	bindables.push_back(std::make_unique<ConstantPS<Buffer>>(buffer, 0));
	pixBuffer = (static_cast<ConstantPS<Buffer>*>(bindables[bindables.size() - 1].get()));

	CreateTransform();

	bindables.push_back(std::make_unique<Texture>("TestModels\\Sprite\\Walk (1).png", 0));
	bindables.push_back(std::make_unique<Sampler>(0));

	bindables.push_back(std::make_unique<PixShader>(L"TexLitPix.cso"));
	bindables.push_back(std::make_unique<VertShader>(L"TexLitVert.cso"));
	ID3DBlob* blob = (static_cast<VertShader*>(bindables[bindables.size() - 1].get()))->GetBlob();

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	bindables.push_back(std::make_unique<InputLayout>(inputLayoutDesc, blob));
}

void TestLitPlate::Update()
{
	ImGui::Begin("BestPlane", &guiWindowOpen);
	ImGui::DragFloat3("Position", &transform.position.x, 0.1f, -10.0f, 10.0f, "%.3f", 0);
	if (ImGui::DragFloat3("Rotation", direction, 0.01f, -1.0f, 1.0f, "%.3f", 0)) {
		transform.rotation = Quat(direction[0] * rhyM::PI, direction[1] * rhyM::PI, direction[2] * rhyM::PI);
	}
	ImGui::End();

	Gameobject::Update();
}

void TestLitPlate::Draw()
{
	UpdateBuffer();
	pixBuffer->SetAndUpdate(buffer);

	Gameobject::Draw();
}

void TestLitPlate::UpdateBuffer()
{
	for (int i = 0; i < 10; i++)
	{
		if (lights[i] == nullptr) {
			buffer.lightAmount = i;
			return;
		}

		buffer.lightNormal[i] = lights[i]->direction;
		buffer.lightPos[i] = lights[i]->transform.position;
	}

	buffer.lightAmount = 10;
}
