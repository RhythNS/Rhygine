#pragma once
#include <vector>
#include <memory>

#include "Scene.h"
#include "Gameobject.h"

class TestScene : public Scene
{
public:
	void Init();
	void Update();
	void Draw();
private:
	std::vector<std::unique_ptr<Gameobject>> gameobjects;
};

