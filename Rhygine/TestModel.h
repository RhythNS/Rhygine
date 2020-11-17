#pragma once
#include "GameObjectFactory.h"

/// <summary>
/// Factory for building a test model.
/// </summary>
class TestModel : public GameObjectFactory
{
public:
	void AddData(GameObject* toAddTo);
};
