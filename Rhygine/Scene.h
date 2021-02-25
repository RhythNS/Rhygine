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
	virtual ~Scene() { }

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
	/// <summary>
	/// Called when the window resizes.
	/// </summary>
	/// <param name="newWidth">The new width of the window.</param>
	/// <param name="newHeight">The new height of the window.</param>
	virtual void InnerOnResize(int newWidth, int newHeight) {}

	std::unique_ptr<Stage> stage;
	Window* window = nullptr;
	float clearColor[4] = { 0,0,0,0 };
private:
	/// <summary>
	/// Called when the window resizes.
	/// </summary>
	/// <param name="newWidth">The new width of the window.</param>
	/// <param name="newHeight">The new height of the window.</param>
	void OnResize(int newWidth, int newHeight);

	/// <summary>
	/// Called when the mouse moves.
	/// </summary>
	/// <param name="x">The new x coordinate of the mouse.</param>
	/// <param name="y">The new y coordinate of the mouse.</param>
	void OnMouseMove(int x, int y);

	/// <summary>
	/// Called when the left mouse button was pressed.
	/// </summary>
	void OnMouseDown();

	/// <summary>
	/// Called when the left mouse button was released.
	/// </summary>
	void OnMouseUp();

	/// <summary>
	/// Inits this scene and adds survivors from the previous scene to this scene.
	/// </summary>
	/// <param name="prevScene">The previous scene that was loaded.</param>
	void OnSceneChange(Scene* prevScene);
};
