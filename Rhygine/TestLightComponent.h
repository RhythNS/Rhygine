#pragma once
#include "Component.h"
#include "Updatable.h"
#include "RhyMath.h"

class Transform;

/// <summary>
/// A test component that represents a light in a scene.
/// </summary>
class TestLightComponent : public Component, public Updatable
{
public:
	void Init();
	void Update();

	int id = 0;
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
private:
	bool guiWindowOpen = true;
	Transform* transform = nullptr;
};

