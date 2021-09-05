#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>
#include <CC/TestLightComponent.h>
#include <RhyMath.h>

class TestLight;
class RotateAround;

/// <summary>
/// A test component that represents a lit object in a scene.
/// </summary>
class TestLitPlateComponent : public Component, public Updatable
{
public:
	void Init();
	void Update();

	void SetLight(TestLightComponent* light);

private:
	float direction[3] = {0.0f, 0.0f, 0.0f};
	bool guiWindowOpen;
	Transform* transform;
};
