#pragma once

class Window;

class Scene
{
	friend class Window;
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	float* GetClearColor();
protected:
	Window* window;
	float clearColor[4] = { 0,0,0,0 };
};

