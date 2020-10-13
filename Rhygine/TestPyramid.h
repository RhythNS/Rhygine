#pragma once
#pragma once
#include <vector>
#include <memory>

#include "GameObject.h"
#include "RhyMath.h"

class Bindable;

class TestPyramid : public GameObject
{
public:
	TestPyramid(Vec3 position);
	void Init();
	void Update();
};
