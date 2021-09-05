#pragma once
#include <ECS/GameObjectFactory.h>
#include <RhyMath.h>

/// <summary>
/// Factory for building a light
/// </summary>
class TestLight : public GameObjectFactory
{
public:
	void AddData(GameObject* toAddTo);
};
