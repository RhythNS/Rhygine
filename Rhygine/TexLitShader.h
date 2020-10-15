#pragma once
#include "CombinedShader.h"
#include "Updatable.h"
#include "ConstantBuffer.h"
#include "TestLightComponent.h"

#include <DirectXMath.h>
#include <memory>
#include <array>

class TexLitShader : public CombinedShader, public Updatable
{
public:
	void Init();
	void Update();

	struct LightInfo {
		float ambientStrength;
		float paddingA[3];

		float lightColor[4];

		float lightPosition[3];
		float paddingB;

		float specStrength;
		float paddingC[3];

		float cameraPos[3];
		float paddingD;
	};

	struct PositionInfo {
		DirectX::XMMATRIX worldPos;
		DirectX::XMMATRIX localScaleRotation;
	};

	std::array<TestLightComponent*, 10> lights;
protected:
	void InnerBind();

private:
	void UpdateLightInfo();
	void UpdatePositionInfo();

	LightInfo lightBuffer;
	PositionInfo posBuffer;

	std::unique_ptr<ConstantPS<LightInfo>> pixBuffer;
	std::unique_ptr<ConstantVS<PositionInfo>> worldPosBuffer;
};

