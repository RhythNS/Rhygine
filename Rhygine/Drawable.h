#pragma once

/// <summary>
/// Interface for classes to implement the Draw function. Draw is called
/// after the Update method and is meant to draw anything to the screen.
/// </summary>
class Drawable
{
public:
	virtual void Draw() {};
};
