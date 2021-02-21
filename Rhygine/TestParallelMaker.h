#pragma once
#include "Component.h"
#include "ParallelUpdatable.h"

class Stage;

class TestParallelMaker : public Component, public ParallelUpdatable
{
public:
	void Update();
	static void AddToStage(Stage* stage);
};

