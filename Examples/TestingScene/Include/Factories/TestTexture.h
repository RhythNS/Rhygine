#pragma once
#include <ECS/GameObjectFactory.h>
#include <ECS/Bindable/Texture.h>
#include <RhyWin.h>

#include <vector>
#include <memory>

/// <summary>
/// Factory for building a plane with a test texture on it.
/// </summary>
class TestTexture : public GameObjectFactory
{
public:
	void AddData(GameObject* toAddTo);
};
