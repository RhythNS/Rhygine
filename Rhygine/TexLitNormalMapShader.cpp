#include "TexLitNormalMapShader.h"

TexLitNormalMapShader::TexLitNormalMapShader(std::string texturePath, std::string normalMapPath)
{
	texture = std::make_unique<Texture>(texturePath.c_str(), 0);
	normalMap = std::make_unique<Texture>(normalMapPath.c_str(), 1);
	sampler = std::make_unique<Sampler>(0);
}

TexLitNormalMapShader::TexLitNormalMapShader(std::string texturePath, std::string normalMapPath, D3D11_SAMPLER_DESC samplerDesc)
{
	texture = std::make_unique<Texture>(texturePath.c_str(), 0);
	normalMap = std::make_unique<Texture>(normalMapPath.c_str(), 1);
	sampler = std::make_unique<Sampler>(0, samplerDesc);
}

void TexLitNormalMapShader::Init()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Tangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Binormal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CreateShaders(L"PhongTexNormalMappingPix.hlsl", L"PhongTexNormalMappingVert.hlsl", &inputLayoutDesc);

	InitBindable(sampler.get());
	InitBindable(texture.get());
	InitBindable(normalMap.get());

	pixBuffer = std::make_unique<ConstantPS<LightInfo>>(&lightBuffer, 0);
	InitBindable<ConstantPS<LightInfo>>(pixBuffer.get());

	worldPosBuffer = std::make_unique<ConstantVS<PositionInfo>>(&posBuffer, 0);
	InitBindable<ConstantVS<PositionInfo>>(worldPosBuffer.get());
}

void TexLitNormalMapShader::InnerBind()
{
	TexLitShader::InnerBind();
	texture->Bind();
	normalMap->Bind();
	sampler->Bind();
}
