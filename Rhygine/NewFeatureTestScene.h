#pragma once
#include "Scene.h"
#include "ParallelPrint.h"

class NewFeatureTestScene : public Scene
{
public:
	void InnerInit();
private:
	ParallelPrint print;
};
