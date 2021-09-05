#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>

class ChangeSceneTest : public Component, public Updatable
{
public:
	void Init();
	void Update();
};

