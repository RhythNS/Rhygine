#pragma once
#include "ConstantBuffer.h"
#include "RhyMath.h"
#include "Component.h"
#include "Updatable.h"
#include "TestLightComponent.h"

#include <array>

class TestLight;

class TestLitPlateComponent : public Component, public Updatable
{
public:
	struct Buffer {
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

	struct VertData {
		DirectX::XMMATRIX worldPos;
		DirectX::XMMATRIX localScaleRotation;
	};

	void Init();
	void Update();

	void SetLight(int index, TestLightComponent* light);

private:
	Buffer buffer;
	VertData vertData;

	bool guiWindowOpen;
	float direction[3] = { 0.0f, 0.0f, 0.0f };
	ConstantPS<Buffer>* pixBuffer;
	ConstantVS<VertData>* worldPosBuffer; 
	
	Transform* transform;
};

