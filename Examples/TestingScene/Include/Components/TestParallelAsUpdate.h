#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>

class Stage;

class TestParallelAsUpdate : public Component, public Updatable
{
public:
	void Update();
	static void AddToStage(Stage* stage);
};
