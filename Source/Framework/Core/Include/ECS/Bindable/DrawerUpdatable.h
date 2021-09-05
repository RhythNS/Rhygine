#pragma once
#include <ECS/Bindable/Drawer.h>

class DrawerUpdatable
{
public:
	virtual void Update(Drawer* drawer) = 0;
};
