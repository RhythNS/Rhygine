#pragma once
#include "GameObjectFactory.h"
#include "TestLightComponent.h"

class TestCrystal : public GameObjectFactory
{
public:
	TestCrystal(TestLightComponent* tlc);
	void AddData(GameObject* toAddTo);
	TestLightComponent* tlc;
};
