#pragma once
#include "Scene.h"

#include "TestPyramid.h"

class TestScene : public Scene
{
public:
	void Init();
	void Update();
	void Draw();
private:
	TestPyramid pyramid;
};

