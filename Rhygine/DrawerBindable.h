#pragma once

class Drawer;

class DrawerBindable 
{
	friend class Drawer;
public:
	Drawer* GetDrawer();
	virtual void AfterDrawerSet() = 0;
private:
	Drawer* drawer;
};