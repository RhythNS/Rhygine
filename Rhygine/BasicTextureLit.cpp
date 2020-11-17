#include "BasicTextureLit.h"

#include "Gameobject.h"
#include "Transform.h"
#include "Drawer.h"

BasicTextureLit::BasicTextureLit(std::string texturePath)
{
	texture = std::make_unique<Texture>(texturePath.c_str(), 0);
}

void BasicTextureLit::Init()
{
	InitBindable(texture.get());

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CreateShaders(L"BasicLitPix.hlsl", L"BasicLitVert.hlsl", &inputLayoutDesc);
	
	pixBuffer = std::make_unique<ConstantPS<LightInfo>>(&lightBuffer, 0);
	InitBindable(pixBuffer.get());

	vertBuffer = std::make_unique<ConstantVS<PositionInfo>>(&posBuffer, 0);
	InitBindable(vertBuffer.get());
}

void BasicTextureLit::Update()
{
	posBuffer.projection = GetPerspectiveMatrix();
	posBuffer.worldPos = DirectX::XMMatrixTranspose(GetWorldMatrix());
	posBuffer.localScaleRotation = GetLocalMatrix();

	vertBuffer->SetAndUpdate(&posBuffer);


	lightBuffer.ambientStrength = ambientStrength;
	
	float* lightColor = light->color;
	lightBuffer.lightColor[0] = lightColor[0];
	lightBuffer.lightColor[1] = lightColor[1];
	lightBuffer.lightColor[2] = lightColor[2];
	lightBuffer.lightColor[3] = lightColor[3];

	RhyM::Vec3* lightPos = &light->GetGameObject()->GetComponent<Transform>()->localPosition;
	lightBuffer.lightPosition[0] = lightPos->m_floats[0];
	lightBuffer.lightPosition[1] = lightPos->m_floats[1];
	lightBuffer.lightPosition[2] = lightPos->m_floats[2];
	pixBuffer->SetAndUpdate(&lightBuffer);
}

void BasicTextureLit::InnerBind()
{
	pixBuffer->Bind();
	vertBuffer->Bind();
	texture->Bind();
}
