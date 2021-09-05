#pragma once
#include <ECS/Bindable/CombinedShader.h>
#include <ECS/Updatable.h>
#include <CC/TestLightComponent.h>
#include <ECS/Bindable/ConstantBuffer.h>

/// <summary>
/// Shader for a toon effect.
/// </summary>
class ToonShader : public CombinedShader, public Updatable 
{
public:
	ToonShader();
	void Update(Drawer* drawer);
	void InnerBind();

	/// <summary>
	/// Info about the position of the gameobject.
	/// </summary>
	struct PositionInfo {
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX worldPos;
		DirectX::XMMATRIX localScaleRotation;
	};

	/// <summary>
	/// Info about the lights in the scene.
	/// </summary>
	struct LightInfo {
		float lightColor[4];

		float lightPosition[3];
		float ambientStrength;

		int levels;
		float paddingA[3];
	};

	TestLightComponent* light;
	float ambientStrength = 0.2f;
	int levels = 6;

private:
	LightInfo lightInfo{};
	PositionInfo positionInfo{};

	std::unique_ptr<ConstantPS<LightInfo>> pixBuffer = nullptr;
	std::unique_ptr<ConstantVS<PositionInfo>> worldPosBuffer = nullptr;
};
