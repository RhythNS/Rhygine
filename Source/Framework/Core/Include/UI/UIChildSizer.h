#pragma once
#include <RhyMath.h>
#include <Math/Rect.h>

class UIElement;
class UIChildSizer
{
	friend class UIElement;
public:
	virtual ~UIChildSizer() { }

protected:
	/// <summary>
	/// Called when the sizer should resize the children of its UIElement.
	/// This method is only called when isControllingChildren returns true.
	/// </summary>
	/// <param name="currentWorldScale">The current world scale. (Scale multiplies by all parents recursivly)</param>
	virtual void ResizeChildren(RhyM::Vec2 currentWorldScale) = 0;
	/// <summary>
	/// Called when the sizer should reposition the children of its UIElement.
	/// This method is only called when isControllingChildren returns true.
	/// </summary>
	virtual void UpdatePositionChildren() = 0;

	/// <summary>
	/// Gets the amount of children that the UIElement has.
	/// </summary>
	int GetChildCount();
	/// <summary>
	/// Gets the bounds rect of the UIElement.
	/// </summary>
	RhyM::Rect& GetRect();
	/// <summary>
	/// Gets the bounds rect of the specified child of the UIElement.
	/// </summary>
	RhyM::Rect& GetRectChild(int at);
	/// <summary>
	/// Gets the bounds rect of the parent of the UIElement.
	/// </summary>
	RhyM::Rect GetParentRect();

	UIElement* element = nullptr;
};
