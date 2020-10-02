#pragma once
#include <vector>
#include <memory>

#include "Scene.h"
#include "Gameobject.h"

class TestScene : public Scene
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Draw();
protected:
	std::vector<std::unique_ptr<Gameobject>> gameobjects;
};

