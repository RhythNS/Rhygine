#pragma once
#include <ECS/GameObjectFactory.h>
#include <CC/TestLightComponent.h>
#include <ECS/Bindable/Texture.h>

/// <summary>
/// Factory for building a cube with the BasicTextureLit shader.
/// </summary>
class TestBasicLit : public GameObjectFactory
{
public:
	TestBasicLit(std::string imagePath, TestLightComponent* tlc);
	TestBasicLit(Texture* texture, TestLightComponent* tlc);
	void AddData(GameObject* toAddTo);

	TestLightComponent* tlc;
	std::string imagePath{};
	Texture* texture = nullptr;
};
