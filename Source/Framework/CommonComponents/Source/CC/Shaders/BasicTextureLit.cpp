#include <CC/Shaders/BasicTextureLit.h>

#include <ECS/GameObject.h>
#include <ECS/Components/Transform.h>
#include <ECS/Bindable/Drawer.h>


BasicTextureLit::BasicTextureLit(std::string texturePath) : texture(std::make_unique<Texture>(texturePath.c_str(), 0))
{
	Init();
}

BasicTextureLit::BasicTextureLit(Texture* texture) : texture(std::make_unique<Texture>(texture, 0))
{
	Init();
}

void BasicTextureLit::Init()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CreateShaders(L"Assets\\Shader\\BasicLitPix.cso", L"Assets\\Shader\\BasicLitVert.cso", &inputLayoutDesc);

	pixBuffer = std::make_unique<ConstantPS<LightInfo>>(&lightBuffer, 0);
	vertBuffer = std::make_unique<ConstantVS<PositionInfo>>(&posBuffer, 0);
}

void BasicTextureLit::Update(Drawer* drawer)
{
	posBuffer.projection = GetPerspectiveMatrix(drawer);
	posBuffer.worldPos = DirectX::XMMatrixTranspose(GetWorldMatrix(drawer));
	posBuffer.localScaleRotation = GetLocalMatrix(drawer);

	vertBuffer->SetAndUpdate(&posBuffer);


	lightBuffer.ambientStrength = ambientStrength;

	float* lightColor = light->color;
	lightBuffer.lightColor[0] = lightColor[0];
	lightBuffer.lightColor[1] = lightColor[1];
	lightBuffer.lightColor[2] = lightColor[2];
	lightBuffer.lightColor[3] = lightColor[3];

	RhyM::Vec3* lightPos = &light->GetGameObject()->GetComponent<Transform>()->localPosition;
	lightBuffer.lightPosition[0] = lightPos->x;
	lightBuffer.lightPosition[1] = lightPos->y;
	lightBuffer.lightPosition[2] = lightPos->z;
	pixBuffer->SetAndUpdate(&lightBuffer);
}

void BasicTextureLit::InnerBind()
{
	pixBuffer->Bind();
	vertBuffer->Bind();
	texture->Bind();
}
