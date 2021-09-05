#include <CC/Shaders/ToonShader.h>
#include <ECS/GameObject.h>
#include <ECS/Components/Transform.h>

ToonShader::ToonShader()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CreateShaders(L"Assets\\Shader\\ToonPix.cso", L"Assets\\Shader\\ToonVert.cso", &inputLayoutDesc);

	pixBuffer = std::make_unique<ConstantPS<LightInfo>>(&lightInfo, 0);
	worldPosBuffer = std::make_unique<ConstantVS<PositionInfo>>(&positionInfo, 0);
}

void ToonShader::Update(Drawer* drawer)
{
	positionInfo.projection = GetPerspectiveMatrix(drawer);
	positionInfo.worldPos = DirectX::XMMatrixTranspose(GetWorldMatrix(drawer));
	positionInfo.localScaleRotation = GetLocalMatrix(drawer);

	worldPosBuffer->SetAndUpdate(&positionInfo);

	float* lightColor = light->color;

	lightInfo.ambientStrength = ambientStrength;
	lightInfo.levels = levels;
	lightInfo.lightColor[0] = lightColor[0];
	lightInfo.lightColor[1] = lightColor[1];
	lightInfo.lightColor[2] = lightColor[2];
	lightInfo.lightColor[3] = lightColor[3];

	RhyM::Vec3* lightPos = &light->GetGameObject()->GetComponent<Transform>()->localPosition;
	lightInfo.lightPosition[0] = lightPos->x;
	lightInfo.lightPosition[1] = lightPos->y;
	lightInfo.lightPosition[2] = lightPos->z;

	pixBuffer->SetAndUpdate(&lightInfo);
}

void ToonShader::InnerBind()
{
	pixBuffer->Bind();
	worldPosBuffer->Bind();
}
