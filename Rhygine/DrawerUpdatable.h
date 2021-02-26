#pragma once
#include "Drawer.h"

class DrawerUpdatable
{
public:
	virtual void Update(Drawer* drawer) = 0;
};
