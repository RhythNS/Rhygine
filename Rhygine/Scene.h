#pragma once
#include "Stage.h"

#include <memory>

class Window;

/// <summary>
/// Represents a gamestate.
/// </summary>
class Scene
{
	friend class Window;
public:
	/// <summary>
	/// Called once to initialize.
	/// </summary>
	void Init();
	/// <summary>
	/// Called every frame. Used to update gameobjects and the gamestate.
	/// </summary>
	void Update();
	/// <summary>
	/// Called every frame. Used to draw gameobjects to the screen.
	/// </summary>
	void Draw();

	/// <summary>
	/// Called after the stage has initialized. 
	/// </summary>
	virtual void InnerInit() {}
	/// <summary>
	/// Called every update before gameobjects are updated.
	/// </summary>
	virtual void InnerPreUpdate() {}
	/// <summary>
	/// Called every update after gameobjects have been updated.
	/// </summary>
	virtual void InnerAfterUpdate() {}
	/// <summary>
	/// Called every draw before gameobjects are drawn.
	/// </summary>
	virtual void InnerPreDraw() {}
	/// <summary>
	/// Called every draw after gameobjects have been drawn.
	/// </summary>
	virtual void InnerAfterDraw() {}

	/// <summary>
	/// Gets the reference to the clear color.
	/// </summary>
	/// <returns>A float array with the size of 4. (r,g,b,a)</returns>
	float* GetClearColor();

protected:
	std::unique_ptr<Stage> stage;
	Window* window;
	float clearColor[4] = { 0,0,0,0 };
};
