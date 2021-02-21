#pragma once
#include "Component.h"
#include "Updatable.h"

class Stage;

class TestParallelAsUpdate : public Component, public Updatable
{
public:
	void Update();
	static void AddToStage(Stage* stage);
};
