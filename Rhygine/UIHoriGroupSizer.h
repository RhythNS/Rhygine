#pragma once
#include "UIChildSizer.h"

/// <summary>
/// Resizes and reposition its children so they form a horizontal list inside the
/// bounds of the UIElement.
/// </summary>
class UIHoriGroupSizer : public UIChildSizer
{
public:
	UIHoriGroupSizer();
	/// <summary>
	/// Padding constructor.
	/// </summary>
	/// <param name="paddingX>Keept space left and right.</param>
	/// <param name="paddingY">Keept space up and down of each UIElement.</param>
	UIHoriGroupSizer(float paddingX, float paddingY);
	virtual ~UIHoriGroupSizer() { }

	void ResizeChildren(RhyM::Vec2 currentWorldScale);
	void UpdatePositionChildren();

	float paddingX = 0, paddingY = 0;
};
