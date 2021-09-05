#pragma once
#include <CC/Shaders/TexLitShader.h>
#include <ECS/Bindable/Texture.h>
#include <ECS/Bindable/Sampler.h>

/// <summary>
/// Shader for phong based shading with normal mapping.
/// </summary>
class TexLitNormalMapShader : public TexLitShader
{
public:
	TexLitNormalMapShader(std::string texturePath, std::string normalMapPath);
	TexLitNormalMapShader(std::string texturePath, std::string normalMapPath, D3D11_SAMPLER_DESC samplerDesc);
	virtual ~TexLitNormalMapShader() { }

	void Init();

protected:
	void InnerBind();

	std::unique_ptr<Texture> texture, normalMap;
	std::unique_ptr<Sampler> sampler;
};
