#pragma once
#include <ECS/GameObjectFactory.h>
#include <CC/TestLightComponent.h>

/// <summary>
/// Factory for building a gameobject with a Phong shader
/// </summary>
class TestLitPlate : public GameObjectFactory
{
public:
	TestLitPlate(TestLightComponent* tlc);
	virtual void AddData(GameObject* toAddTo);
private:
	TestLightComponent* tlc;
};

