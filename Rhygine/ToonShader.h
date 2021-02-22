#pragma once
#include "CombinedShader.h"
#include "Updatable.h"
#include "TestLightComponent.h"
#include "ConstantBuffer.h"

/// <summary>
/// Shader for a toon effect.
/// </summary>
class ToonShader : public CombinedShader, public Updatable 
{
public:
	void AfterDrawerSet();
	void Update();
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
