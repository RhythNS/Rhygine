#pragma once
#include "RhyMath.h"
#include "Rect.h"

class UIElement;
class UISizer
{
	friend class UIElement;

public:
	enum class VertAlignment {
		Up, Middle, Down
	};
	enum class HoriAlignment {
		Left, Middle, Right
	};
	virtual ~UISizer() { }

protected:
	/// <summary>
	/// Called when the sizer should resize the UIElement.
	/// </summary>
	/// <param name="currentWorldScale">The current world scale. (Scale multiplies by all parents recursivly)</param>
	void OnResize(RhyM::Vec2 currentWorldScale);
	/// <summary>
	/// Called when the sizer should update the UIElement.
	/// </summary>
	void OnUpdatePosition();
	
	/// <summary>
	/// Returns true when the sizer is controlling the size and position of the children.
	/// If this is set to true, then ResizeSelf and UpdatePositionSelf should not be called on the children
	/// of the UIElement that this sizer is on.
	/// </summary>
	virtual const bool isControllingChildren() = 0;
	
	/// <summary>
	/// Called when sizer should resize the UIElement itself.
	/// </summary>
	/// <param name="rect">The bounds of the UIElement.</param>
	/// <param name="currentWorldScale">The current world scale. (Scale multiplies by all parents recursivly)</param>
	virtual void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	/// <summary>
	/// Called when the sizer should update the position of the UIElements.
	/// </summary>
	/// <param name="rect"></param>
	virtual void UpdatePositionSelf(RhyM::Rect& rect);

	/// <summary>
	/// Helper method for updating the position based given parameters.
	/// </summary>
	/// <param name="rect">The bounds of the UIElement.</param>
	/// <param name="vert">The vertical alignment.</param>
	/// <param name="hori">The horizontal alignment</param>
	void UpdatePositionDefaultHoriVert(RhyM::Rect& rect, VertAlignment vert, HoriAlignment hori, float paddingX = 0.0f, float paddingY = 0.0f);
	
	/// <summary>
	/// Called when the sizer should resize the children of its UIElement.
	/// This method is only called when isControllingChildren returns true.
	/// </summary>
	/// <param name="currentWorldScale">The current world scale. (Scale multiplies by all parents recursivly)</param>
	virtual void ResizeChildren(RhyM::Vec2 currentWorldScale) {}
	/// <summary>
	/// Called when the sizer should reposition the children of its UIElement.
	/// This method is only called when isControllingChildren returns true.
	/// </summary>
	virtual void UpdatePositionChildren() {}

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
	/// <returns></returns>
	RhyM::Rect GetParentRect();

	UIElement* element = nullptr;
};
