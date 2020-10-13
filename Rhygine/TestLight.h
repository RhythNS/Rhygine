#pragma once
#include "GameObject.h"
#include "RhyMath.h"

class TestLight : public GameObject
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
