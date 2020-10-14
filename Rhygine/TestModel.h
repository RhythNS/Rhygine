#pragma once
#include "GameObjectFactory.h"

class TestModel : public GameObjectFactory
{
public:
	void AddData(GameObject* toAddTo);
};
