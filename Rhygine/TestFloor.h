#pragma once
#include "GameObjectFactory.h"
#include "TestLightComponent.h"

/// <summary>
/// Factory for building a gameobject with a PhongTexNormalMapping shader.
/// </summary>
class TestFloor : public GameObjectFactory
{
public:
	TestFloor(float width, float depth, TestLightComponent* tlc);
	void AddData(GameObject* toAddTo);
	float width, depth;
	TestLightComponent* tlc;
};
