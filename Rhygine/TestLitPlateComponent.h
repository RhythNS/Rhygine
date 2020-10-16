#pragma once
#include "ConstantBuffer.h"
#include "RhyMath.h"
#include "Component.h"
#include "Updatable.h"
#include "TestLightComponent.h"

class TestLight;

class TestLitPlateComponent : public Component, public Updatable
{
public:
	void Init();
	void Update();

	void SetLight(int index, TestLightComponent* light);

private:
	float direction[3] = {0.0f, 0.0f, 0.0f};

	bool guiWindowOpen;
	Transform* transform;
};

