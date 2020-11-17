#pragma once
#pragma once
#include <vector>
#include <memory>

#include "GameObjectFactory.h"
#include "RhyMath.h"

class Bindable;

/// <summary>
/// Factory for building a transparent pyramid.
/// </summary>
class TestPyramid : public GameObjectFactory
{
public:
	void AddData(GameObject* toAddTo);
};
