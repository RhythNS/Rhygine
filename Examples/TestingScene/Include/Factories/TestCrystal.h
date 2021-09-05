#pragma once
#include <ECS/GameObjectFactory.h>
#include <CC/TestLightComponent.h>

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
