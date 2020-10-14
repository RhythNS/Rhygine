#include "TestLitPlateComponent.h"
#include "RhyBindables.h"
#include "Rhyimgui.h"
#include "TestLight.h"
#include "Window.h"
#include "Gfx.h"
#include "Transform.h"
#include "Drawer.h"


void TestLitPlateComponent::Init()
{
	transform = GetGameObject()->AddComponent<Transform>();
	Drawer* drawer = GetGameObject()->AddComponent<Drawer>();

	transform->position.Set(0.0f, 0.0f, 5.0f);

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

	drawer->AddBindable(std::make_unique<VertBuffer<Vertex>>(verts, 0));
	drawer->AddBindable(std::make_unique<IndexBufferUS>(indexes, 0));
	drawer->AddBindable(std::make_unique<PrimitiveTopolpgy>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	drawer->AddBindable(std::make_unique<ConstantPS<Buffer>>(buffer, 0, &TestLitPlateComponent::UpdateBuffer));
	drawer->AddBindable(std::make_unique<Texture>("TestModels\\Sprite\\TestGradient.png", 0));
	drawer->AddBindable(std::make_unique<Sampler>(0));

	drawer->AddBindable(std::make_unique<PixShader>(L"TexLitPix.hlsl"));
	drawer->AddBindable(std::make_unique<VertShader>(L"TexLitVert.hlsl"));

	ID3DBlob* blob = drawer->GetBindable<VertShader>()->GetBlob();

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	drawer->AddBindable(std::make_unique<InputLayout>(inputLayoutDesc, blob));

	drawer->AddBindable(std::make_unique<ConstantVS<VertData>>(vertData, 1, &TestLitPlateComponent::UpdateVertBuffer));
}

void TestLitPlateComponent::Update()
{
	ImGui::Begin("BestPlane", &guiWindowOpen);
	ImGui::DragFloat3("Position", &transform->position.x, 0.1f, -10.0f, 10.0f, "%.3f", 0);
	if (ImGui::DragFloat3("Rotation", direction, 0.01f, -1.0f, 1.0f, "%.3f", 0)) {
		transform->rotation = Quat(direction[0] * rhyM::PI, direction[1] * rhyM::PI, direction[2] * rhyM::PI);
	}
	ImGui::End();
}

bool TestLitPlateComponent::UpdateBuffer(ConstantBuffer<Buffer>* cb)
{
	buffer.ambientStrength = 0.2f;
	buffer.specStrength = 1.0f;

	buffer.lightColor[0] = 1.0f;
	buffer.lightColor[1] = 1.0f;
	buffer.lightColor[2] = 0.6f;
	buffer.lightColor[3] = 1.0f;

	Vec3* lightPos = &lights[0]->GetGameObject()->GetComponent<Transform>()->position;
	buffer.lightPosition[0] = lightPos->x;
	buffer.lightPosition[1] = lightPos->y;
	buffer.lightPosition[2] = lightPos->z;

	TestCamera::Vector* cameraPos = &Window::GetInstance()->GetGfx()->camera.position;
	buffer.cameraPos[0] = cameraPos->x;
	buffer.cameraPos[1] = cameraPos->y;
	buffer.cameraPos[2] = cameraPos->z;

	cb->Set(buffer);

	return true;
}

bool TestLitPlateComponent::UpdateVertBuffer(ConstantBuffer<VertData>* cb)
{
	vertData.worldPos = *transform->GetWorldMatrix();
	vertData.localScaleRotation = *transform->GetLocalMatrix();

	cb->Set(vertData);

	return true;
}