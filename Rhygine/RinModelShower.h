#pragma once
#include "Component.h"
#include "Updatable.h"
#include "Drawer.h"

#include <vector>

class RinModelShower : public Component, public Updatable
{
public:
	void Set(std::vector<Drawer*> drawers);
	void Update();

private:
	std::vector<Drawer*> drawers;
};
