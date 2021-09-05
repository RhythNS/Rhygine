#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>
#include <ECS/Bindable/Drawer.h>

#include <vector>

class RinModelShower : public Component, public Updatable
{
public:
	void Set(std::vector<Drawer*> drawers);
	void Update();

private:
	std::vector<Drawer*> drawers;
};
