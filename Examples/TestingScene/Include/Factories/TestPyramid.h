#pragma once
#include <ECS/GameObjectFactory.h>
#include <RhyMath.h>

#include <vector>
#include <memory>

class Bindable;

/// <summary>
/// Factory for building a transparent pyramid.
/// </summary>
class TestPyramid : public GameObjectFactory
{
public:
	void AddData(GameObject* toAddTo);
};
