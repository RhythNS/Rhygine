#pragma once
#include "CombinedShader.h"
#include "Updatable.h"
#include "TexLitShader.h"
#include "Texture.h"
#include "Sampler.h"
#include "TestLightComponent.h"

/// <summary>
/// Shader for basic lighting.
/// </summary>
class BasicTextureLit : public CombinedShader, public Updatable
{
public:
	BasicTextureLit(std::string texturePath);
	virtual ~BasicTextureLit() { }

	void Init();
	void Update();
	void InnerBind();

	struct LightInfo {
		float lightColor[4];

		float lightPosition[3];
		float ambientStrength;
	};

	struct PositionInfo {
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX worldPos;
		DirectX::XMMATRIX localScaleRotation;
	};

	float ambientStrength = 0.4f;

	LightInfo lightBuffer{ 0 };
	PositionInfo posBuffer{  };

	TestLightComponent* light;

	std::unique_ptr<ConstantPS<LightInfo>> pixBuffer;
	std::unique_ptr<ConstantVS<PositionInfo>> vertBuffer;
	std::unique_ptr<Texture> texture;
};
