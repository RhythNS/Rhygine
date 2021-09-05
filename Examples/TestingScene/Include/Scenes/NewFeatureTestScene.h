#pragma once
#include <Core/Scene.h>
#include <Components/ParallelPrint.h>

class NewFeatureTestScene : public Scene
{
public:
	void InnerInit();
private:
	ParallelPrint print;
};
