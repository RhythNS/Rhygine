#pragma once
#pragma once
#include <vector>
#include <memory>

#include "Gameobject.h"

class Bindable;

class TestPyramid : public Gameobject
{
public:
	TestPyramid(Vector position);
	void Init();
	void Update();
};
