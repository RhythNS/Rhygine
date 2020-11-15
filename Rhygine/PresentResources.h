#pragma once
#include "Component.h"
#include "MonoFont.h"

class PresentResources : public Component
{
public:
	void Init();

	static PresentResources* instance;

	MonoFont defFont{ "PresentScene\\SourceCodePro_Regular_(72)_[0,0,0,0]_45x76.png", 45, 76 };
};

