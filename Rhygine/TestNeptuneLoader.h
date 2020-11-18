#pragma once
#include "GameObjectFactory.h"
#include "TestLightComponent.h"

/// <summary>
/// Factory for building a complex model.
/// </summary>
class TestNeptuneLoader : public GameObjectFactory
{
public:
	TestNeptuneLoader(TestLightComponent* tlc);
	void AddData(GameObject* toAddTo);
	TestLightComponent* tlc;
};
