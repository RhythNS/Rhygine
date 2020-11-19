#pragma once
#include "Component.h"
#include "Updatable.h"
#include "UIFont.h"

class InfoDisplayer : public Component, public Updatable
{
public:
	void Init();
	void Update();
private:
	UIFont* frameDurationFont = nullptr, *infoFont = nullptr;

	bool displaying = true;

	float lastDurations[100] = {};
	int atDuration = 0;
};
