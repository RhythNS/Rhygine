#pragma once
#include <ECS/Component.h>
#include <ECS/ParallelUpdatable.h>

class Stage;

class TestParallelMaker : public Component, public ParallelUpdatable
{
public:
	void ParallelUpdate();
	static void AddToStage(Stage* stage);
};

