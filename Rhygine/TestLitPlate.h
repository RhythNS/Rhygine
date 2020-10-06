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
		Vec3 lightPos[10];
		Vec3 lightNormal[10];
		UINT lightAmount;
	
		float padding[3];
	};

	TestLitPlate(std::array<TestLight*, 10> lights);

	void Init();
	void Update();
	void Draw();
private:
	void UpdateBuffer();

	std::array<TestLight*, 10>  lights;

	Buffer buffer;
	bool guiWindowOpen;
	float direction[3] = { 0.0f, 0.0f, 0.0f };
	ConstantPS<Buffer>* pixBuffer;
};

