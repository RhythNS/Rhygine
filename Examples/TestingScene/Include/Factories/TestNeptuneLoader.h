#pragma once
#include <ECS/GameObjectFactory.h>
#include <CC/TestLightComponent.h>
#include <ECS/Bindable/Texture.h>

/// <summary>
/// Factory for building a complex model.
/// </summary>
class TestNeptuneLoader : public GameObjectFactory
{
public:
	TestNeptuneLoader(TestLightComponent* tlc);
	void AddData(GameObject* toAddTo);
	TestLightComponent* tlc;

	std::unique_ptr<Texture> texs[4];
};
