#pragma once
#include "Gameobject.h"
#include "ConstantBuffer.h"
#include "RhyMath.h"

#include <array>

class TestLight;

class TestLitPlate : public Gameobject
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

		/*
		Vec3 lightPos;
		Vec3 lightNormal;
		float lightColor[4];

		float ambientStrength;

		float padding;
		*/
	};

	struct VertData {
		DirectX::XMMATRIX worldPos;
		DirectX::XMMATRIX localScaleRotation;
	};

	TestLitPlate(std::array<TestLight*, 10> lights);

	void Init();
	void Update();
	void Draw();
private:
	void UpdateBuffer();
	void UpdateVertBuffer();

	std::array<TestLight*, 10>  lights;

	Buffer buffer;
	VertData vertData;

	bool guiWindowOpen;
	float direction[3] = { 0.0f, 0.0f, 0.0f };
	ConstantPS<Buffer>* pixBuffer;
	ConstantVS<VertData>* worldPosBuffer;
};

