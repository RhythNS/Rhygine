#pragma once

#include "Gameobject.h"

class Testimgui : public Gameobject
{
public:
	void Init();
	void Update();
	void Draw();
private:
	bool show = true;
};

