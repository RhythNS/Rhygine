#pragma once
#include "Gameobject.h"
class TestLight : public Gameobject
{
public:
	TestLight(int id, float* direction[3]);
	void Init();
	void Update();

	int id;
	float direction[3];
private:
	bool guiWindowOpen = true;
};
