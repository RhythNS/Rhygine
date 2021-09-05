#pragma once
#include <ECS/Bindable/CombinedShader.h>
#include <ECS/Updatable.h>
#include <CC/Shaders/TexLitShader.h>
#include <CC/TestLightComponent.h>
#include <ECS/Bindable/Texture.h>
#include <ECS/Bindable/Sampler.h>

/// <summary>
/// Shader for basic lighting.
/// </summary>
class BasicTextureLit : public CombinedShader, public Updatable
{
public:
	BasicTextureLit(std::string texturePath);
	BasicTextureLit(Texture* texture);
	virtual ~BasicTextureLit() { }

	void Init();
	void Update(Drawer* drawer);
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
