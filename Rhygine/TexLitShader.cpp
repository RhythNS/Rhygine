#include "TexLitShader.h"
#include "RhyMath.h"
#include "Window.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Drawer.h"
#include "Rhyimgui.h"

void TexLitShader::Init()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CreateShaders(L"TexLitPix.hlsl", L"TexLitVert.hlsl", &inputLayoutDesc);

	pixBuffer = std::make_unique<ConstantPS<LightInfo>>(&lightBuffer, 0);
	InitBindable<ConstantPS<LightInfo>>(pixBuffer.get());

	worldPosBuffer = std::make_unique<ConstantVS<PositionInfo>>(&posBuffer, 0);
	InitBindable<ConstantVS<PositionInfo>>(worldPosBuffer.get());
}

void TexLitShader::Update()
{
	UpdateLightInfo();
	UpdatePositionInfo();
}

void TexLitShader::InnerBind()
{
	pixBuffer->Bind();
	worldPosBuffer->Bind();
}

void TexLitShader::UpdateLightInfo()
{
	static float ambientStrength = 0.2f;
	static float specStrength = 50.0f;

	static float lightColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	ImGui::Begin("TexLitShader");
	ImGui::DragFloat("Ambient Strength", &ambientStrength, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Spec Strength", &specStrength, 50.0f, 50.0f, 1000.0f);
	ImGui::DragFloat4("Light color", lightColor, 0.01f, 0.0f, 1.0f);
	ImGui::End();

	lightBuffer.ambientStrength = ambientStrength;
	lightBuffer.specStrength = specStrength;

	lightBuffer.lightColor[0] = lightColor[0];
	lightBuffer.lightColor[1] = lightColor[1];
	lightBuffer.lightColor[2] = lightColor[2];
	lightBuffer.lightColor[3] = lightColor[3];

	RhyM::Vec3* lightPos = &lights[0]->GetGameObject()->GetComponent<Transform>()->position;
	lightBuffer.lightPosition[0] = lightPos->x;
	lightBuffer.lightPosition[1] = lightPos->y;
	lightBuffer.lightPosition[2] = lightPos->z;

	RhyM::Vec3* cameraPos = &GetDrawer()->GetCamera()->GetTransform()->position;
	lightBuffer.cameraPos[0] = cameraPos->x;
	lightBuffer.cameraPos[1] = cameraPos->y;
	lightBuffer.cameraPos[2] = cameraPos->z;

	pixBuffer->SetAndUpdate(&lightBuffer);
}

void TexLitShader::UpdatePositionInfo()
{
	posBuffer.projection = *GetPerspectiveMatrix();
	posBuffer.worldPos = *GetWorldMatrix();
	posBuffer.localScaleRotation = *GetLocalMatrix();

	worldPosBuffer->SetAndUpdate(&posBuffer);
}
