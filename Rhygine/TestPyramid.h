#pragma once
#pragma once
#include <vector>
#include <memory>

#include "Gameobject.h"
#include "RhyMath.h"

class Bindable;

class TestPyramid : public Gameobject
{
public:
	TestPyramid(Vec3 position);
	void Init();
	void Update();
};
