#pragma once
#include "Stage.h"

#include <memory>

class Window;

class Scene
{
	friend class Window;
public:
	void Init();
	void Update();
	void Draw();

	virtual void InnerInit() {}
	virtual void InnerPreUpdate() {}
	virtual void InnerAfterUpdate() {}
	virtual void InnerPreDraw() {}
	virtual void InnerAfterDraw() {}

	float* GetClearColor();
protected:
	std::unique_ptr<Stage> stage;
	Window* window;
	float clearColor[4] = { 0,0,0,0 };
};
