#pragma once
#include "GameObjectFactory.h"
#include "TestLightComponent.h"
#include "Texture.h"
#include <memory>

/// <summary>
/// Factory for building a complex model.
/// </summary>
class TestModelLoader : public GameObjectFactory
{
public:
	TestModelLoader(TestLightComponent* tlc);
	void AddData(GameObject* toAddTo);
	TestLightComponent* tlc;

	std::vector<std::unique_ptr<Texture>> texs;
};
