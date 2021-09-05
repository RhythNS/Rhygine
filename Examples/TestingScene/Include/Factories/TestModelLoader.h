#pragma once
#include <ECS/GameObjectFactory.h>
#include <CC/TestLightComponent.h>
#include <ECS/Bindable/Texture.h>

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
};
