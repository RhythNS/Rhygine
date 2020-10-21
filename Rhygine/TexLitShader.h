#pragma once
#include "CombinedShader.h"
#include "Updatable.h"
#include "ConstantBuffer.h"
#include "TestLightComponent.h"

#include <DirectXMath.h>
#include <memory>
#include <array>

/// <summary>
/// A textured phong shader.
/// </summary>
class TexLitShader : public CombinedShader, public Updatable
{
public:
	void Init();
	void Update();

	/// <summary>
	/// Info about the lights in the scene.
	/// </summary>
	struct LightInfo {
		float lightColor[4];

		float lightPosition[3];
		float paddingA;

		float cameraPos[3];
		float specStrength;

		float ambientStrength;
		float paddingB[3];
	};

	/// <summary>
	/// Info about the position of the gameobject.
	/// </summary>
	struct PositionInfo {
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX worldPos;
		DirectX::XMMATRIX localScaleRotation;
	};

	std::array<TestLightComponent*, 10> lights;
protected:
	void InnerBind();

private:
	/// <summary>
	/// Updates the lightinfo buffer.
	/// </summary>
	void UpdateLightInfo();
	/// <summary>
	/// Updates the position buffer.
	/// </summary>
	void UpdatePositionInfo();

	LightInfo lightBuffer;
	PositionInfo posBuffer;

	std::unique_ptr<ConstantPS<LightInfo>> pixBuffer;
	std::unique_ptr<ConstantVS<PositionInfo>> worldPosBuffer;
};

