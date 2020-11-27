#pragma once
#include "GameObjectFactory.h"
#include "TestLightComponent.h"

/// <summary>
/// Factory for building a gameobject with the TexLitNormalMapShader
/// </summary>
class TestCrystal : public GameObjectFactory
{
public:
	TestCrystal(TestLightComponent* tlc);
	void AddData(GameObject* toAddTo);
	TestLightComponent* tlc;
};
