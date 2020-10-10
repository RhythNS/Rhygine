#pragma once
#include "Gameobject.h"
#include "RhyMath.h"

class TestLight : public Gameobject
{
public:
	TestLight(int id, float* direction);
	void Init();
	void Update();

	int id;
	Vec3 direction;
	float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
private:
	bool guiWindowOpen = true;
};
