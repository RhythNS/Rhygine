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
	TexLitShader();
	virtual ~TexLitShader() {}

	virtual void Update(Drawer* drawer);

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

	TestLightComponent* light;
	float ambientStrength = 0.2f;
	float specStrength = 50.0f;
protected:
	virtual void InnerBind();

	/// <summary>
	/// Updates the lightinfo buffer.
	/// </summary>
	virtual void UpdateLightInfo(Drawer* drawer);
	/// <summary>
	/// Updates the position buffer.
	/// </summary>
	virtual void UpdatePositionInfo(Drawer* drawer);

	LightInfo lightBuffer;
	PositionInfo posBuffer;

	std::unique_ptr<ConstantPS<LightInfo>> pixBuffer;
	std::unique_ptr<ConstantVS<PositionInfo>> worldPosBuffer;
};
