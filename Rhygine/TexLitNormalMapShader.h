#pragma once
#include "TexLitShader.h"
#include "Texture.h"
#include "Sampler.h"

/// <summary>
/// Shader for phong based shading with normal mapping.
/// </summary>
class TexLitNormalMapShader : public TexLitShader
{
public:
	TexLitNormalMapShader(std::string texturePath, std::string normalMapPath);
	TexLitNormalMapShader(std::string texturePath, std::string normalMapPath, D3D11_SAMPLER_DESC samplerDesc);
	virtual ~TexLitNormalMapShader() { }

	void AfterDrawerSet();

protected:
	void InnerBind();

	std::unique_ptr<Texture> texture, normalMap;
	std::unique_ptr<Sampler> sampler;
};
