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

	int id;
	RhyM::Vec3 direction;
	float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
private:
	bool guiWindowOpen = true;
	Transform* transform;
};

