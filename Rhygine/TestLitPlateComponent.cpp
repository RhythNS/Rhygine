#include "TestLitPlateComponent.h"
#include "RhyBindables.h"
#include "Rhyimgui.h"
#include "TestLight.h"
#include "Window.h"
#include "Gfx.h"
#include "Transform.h"
#include "Drawer.h"
#include "TexLitShader.h"
#include "RotateAround.h"

void TestLitPlateComponent::Init()
{
	transform = GetGameObject()->AddComponent<Transform>();
	Drawer* drawer = GetGameObject()->AddComponent<Drawer>();

	transform->position.Set(0.0f, 0.0f, 5.0f);
	transform->scale.Set(3.0f, 3.0f, 3.0f);

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

	drawer->AddBindable(std::make_unique<Texture>("TestModels\\Sprite\\TestImage.png", 0));
	drawer->AddBindable(std::make_unique<Sampler>(0));

	drawer->AddBindable(std::make_unique<TexLitShader>());

	rotateAround = GetGameObject()->AddComponent<RotateAround>();
	rotateAround->Disable();
}

void TestLitPlateComponent::Update()
{
	ImGui::Begin("BestPlane", &guiWindowOpen);
	ImGui::DragFloat3("Position", &transform->position.x, 0.1f, -10.0f, 10.0f, "%.3f", 0);
	if (ImGui::DragFloat3("Rotation", direction, 0.01f, -1.0f, 1.0f, "%.3f", 0)) {
		transform->rotation = RhyM::Quat(direction[0] * RhyM::PI, direction[1] * RhyM::PI, direction[2] * RhyM::PI);
	}
	bool boxRotates = rotateAround->IsEnabled();
	if (ImGui::Checkbox("Rotate around", &boxRotates))
	{
		if (boxRotates)
			rotateAround->Enable();
		else
			rotateAround->Disable();
	}
	ImGui::DragFloat3("Rotate speed", &rotateAround->rotationSpeed.x, 0.05f, -2.0, 2.0f);

	ImGui::End();
}

void TestLitPlateComponent::SetLight(int index, TestLightComponent* light)
{
	GetGameObject()->GetComponent<Drawer>()->GetBindable<TexLitShader>()->lights[index] = light;
}
