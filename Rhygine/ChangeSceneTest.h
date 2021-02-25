#pragma once
#include "Component.h"
#include "Updatable.h"

class ChangeSceneTest : public Component, public Updatable
{
public:
	void Init();
	void Update();
};

