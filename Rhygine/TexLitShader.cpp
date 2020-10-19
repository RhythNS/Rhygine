#include "TexLitShader.h"
#include "RhyMath.h"
#include "Window.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Drawer.h"

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
	lightBuffer.ambientStrength = 0.2f;
	lightBuffer.specStrength = 64.0f;

	lightBuffer.lightColor[0] = 1.0f;
	lightBuffer.lightColor[1] = 1.0f;
	lightBuffer.lightColor[2] = 0.6f;
	lightBuffer.lightColor[3] = 1.0f;

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
