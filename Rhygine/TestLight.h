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
private:
	bool guiWindowOpen = true;
};
