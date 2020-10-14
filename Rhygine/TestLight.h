#pragma once
#include "GameObjectFactory.h"
#include "RhyMath.h"

class TestLight : public GameObjectFactory
{
public:
	void AddData(GameObject* toAddTo);
};
