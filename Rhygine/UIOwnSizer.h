#pragma once
#include "RhyMath.h"
#include "Rect.h"

class UIElement;
enum class VertAlignment;
enum class HoriAlignment;
class UIOwnSizer
{
	friend class UIElement;

public:
	virtual ~UIOwnSizer() { }

protected:
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
	/// Gets the bounds rect of the UIElement.
	/// </summary>
	RhyM::Rect& GetRect();
	/// <summary>
	/// Gets the bounds rect of the parent of the UIElement.
	/// </summary>
	RhyM::Rect GetParentRect();

	UIElement* element = nullptr;
};

