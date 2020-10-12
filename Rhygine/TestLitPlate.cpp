#include "TestLitPlate.h"

#include "RhyBindables.h"
#include "Rhyimgui.h"
#include "TestLight.h"
#include "Window.h"
#include "Gfx.h"

TestLitPlate::TestLitPlate(std::array<TestLight*, 10> lights) : lights(lights), guiWindowOpen(true), pixBuffer(nullptr)
{
	//buffer = { Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
}

void TestLitPlate::Init()
{

	transform.position.Set(0.0f, 0.0f, 5.0f);

	struct Vertex {
		struct {
			float x, y, z;
		} pos;
		struct {
			float nx, ny, nz;
		} normal;
		struct {
			float u;
			float v;
		} texCoords;
	};

	std::vector<Vertex> verts =
	{
		/*
		{-1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f},
		{-1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f},
		{ 1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f},
		{ 1.0f, -1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f}
		*/
		//   x     y     z        nx     ny     nz      u      v
		{ -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f }, // 0 VUL
		{  0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f }, // 1 VUR
		{ -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f }, // 2 VOL
		{  0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f }, // 3 VOR
		{  0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f }, // 1 VUR
		{  0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f }, // 5 HUR
		{  0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f }, // 3 VOR
		{  0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f }, // 7 HOR
		{  0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f }, // 5 HUR
		{ -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f }, // 4 HUL
		{  0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f }, // 7 HOR
		{ -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f }, // 6 HOL
		{ -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f }, // 4 HUL
		{ -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f }, // 0 VUL
		{ -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f }, // 6 HOL
		{ -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f }, // 2 VOL
		{ -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f }, // 4 HUL
		{  0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f }, // 5 HUR
		{ -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f }, // 0 VUL
		{  0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f }, // 1 VUR
		{ -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f }, // 2 VOL
		{  0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f }, // 3 VOR
		{ -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f }, // 6 HOL
		{  0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f }, // 7 HOR
	};

	std::vector<unsigned short> indexes =
	{
		/*
		0, 1, 2,
		3, 0, 2
		*/
		0,   2,   1,   2,   3,   1,
		4,   6,   5,   6,   7,   5,
		8,   10,  9,   10,  11,  9,
		12,  14,  13,  14,  15,  13,
		16,  18,  17,  18,  19,  17,
		20,  22,  21,  22,  23,  21,
	};

	bindables.push_back(std::make_unique<VertBuffer<Vertex>>(verts, 0));

	bindables.push_back(std::make_unique<IndexBufferUS>(indexes, 0));
	indexBuffer = static_cast<IndexBufferUS*>(bindables[bindables.size() - 1].get());

	bindables.push_back(std::make_unique<PrimitiveTopolpgy>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	UpdateBuffer();
	bindables.push_back(std::make_unique<ConstantPS<Buffer>>(buffer, 0));
	pixBuffer = (static_cast<ConstantPS<Buffer>*>(bindables[bindables.size() - 1].get()));

	CreateTransform();

	bindables.push_back(std::make_unique<Texture>("TestModels\\Sprite\\TestGradient.png", 0));
	bindables.push_back(std::make_unique<Sampler>(0));

	bindables.push_back(std::make_unique<PixShader>(L"TexLitPix.hlsl"));
	bindables.push_back(std::make_unique<VertShader>(L"TexLitVert.hlsl"));
	ID3DBlob* blob = (static_cast<VertShader*>(bindables[bindables.size() - 1].get()))->GetBlob();

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	bindables.push_back(std::make_unique<InputLayout>(inputLayoutDesc, blob));

	UpdateVertBuffer();
	bindables.push_back(std::make_unique<ConstantVS<VertData>>(vertData, 1));
	worldPosBuffer = (static_cast<ConstantVS<VertData>*>(bindables[bindables.size() - 1].get()));
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
	UpdateVertBuffer();
	worldPosBuffer->SetAndUpdate(vertData);
	UpdateBuffer();
	pixBuffer->SetAndUpdate(buffer);

	Gameobject::Draw();
}

void TestLitPlate::UpdateBuffer()
{
	buffer.ambientStrength = 0.2f;
	buffer.specStrength = 1.0f;

	buffer.lightColor[0] = 1.0f;
	buffer.lightColor[1] = 1.0f;
	buffer.lightColor[2] = 0.6f;
	buffer.lightColor[3] = 1.0f;

	Vec3* lightPos = &lights[0]->transform.position;
	buffer.lightPosition[0] = lightPos->x;
	buffer.lightPosition[1] = lightPos->y;
	buffer.lightPosition[2] = lightPos->z;

	TestCamera::Vector* cameraPos = &Window::GetInstance()->GetGfx()->camera.position;
	buffer.cameraPos[0] = cameraPos->x;
	buffer.cameraPos[1] = cameraPos->y;
	buffer.cameraPos[2] = cameraPos->z;

	// buffer = { Vec3(0,1,2), Vec3(3,4,5), 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f };

	/*
	buffer.lightPos = lights[0]->transform.position;
	buffer.lightNormal = lights[0]->direction;
	for (int i = 0; i < 4; i++)
	{
		buffer.lightColor[i] = lights[0]->color[i];
	}
	buffer.ambientStrength = 1.0f;
	*/

	/*
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
	*/
}

void TestLitPlate::UpdateVertBuffer()
{
	vertData.worldPos = transform.GetWorldMatrix()->transform;
	vertData.localScaleRotation = transform.GetLocalMatrix()->transform;
}
