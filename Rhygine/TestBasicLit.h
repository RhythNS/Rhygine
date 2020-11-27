#pragma once
#include "GameObjectFactory.h"
#include "TestLightComponent.h"
#include "Texture.h"

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
