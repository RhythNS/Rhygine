#include "ToonShader.h"
#include "Gameobject.h"
#include "Transform.h"

void ToonShader::Init()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CreateShaders(L"ToonPix.hlsl", L"ToonVert.hlsl", &inputLayoutDesc);

	pixBuffer = std::make_unique<ConstantPS<LightInfo>>(&lightInfo, 0);
	InitBindable<ConstantPS<LightInfo>>(pixBuffer.get());

	worldPosBuffer = std::make_unique<ConstantVS<PositionInfo>>(&positionInfo, 0);
	InitBindable<ConstantVS<PositionInfo>>(worldPosBuffer.get());
}

void ToonShader::Update()
{
	positionInfo.projection = GetPerspectiveMatrix();
	positionInfo.worldPos = DirectX::XMMatrixTranspose(GetWorldMatrix());
	positionInfo.localScaleRotation = GetLocalMatrix();

	worldPosBuffer->SetAndUpdate(&positionInfo);

	float* lightColor = light->color;

	lightInfo.ambientStrength = ambientStrength;
	lightInfo.levels = levels;
	lightInfo.lightColor[0] = lightColor[0];
	lightInfo.lightColor[1] = lightColor[1];
	lightInfo.lightColor[2] = lightColor[2];
	lightInfo.lightColor[3] = lightColor[3];

	RhyM::Vec3* lightPos = &light->GetGameObject()->GetComponent<Transform>()->localPosition;
	lightInfo.lightPosition[0] = lightPos->m_floats[0];
	lightInfo.lightPosition[1] = lightPos->m_floats[1];
	lightInfo.lightPosition[2] = lightPos->m_floats[2];

	pixBuffer->SetAndUpdate(&lightInfo);
}

void ToonShader::InnerBind()
{
	pixBuffer->Bind();
	worldPosBuffer->Bind();
}
