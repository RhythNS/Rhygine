#pragma once
#include "UISizer.h"

/// <summary>
/// Resizes and reposition its children so they form a horizontal list inside the
/// bounds of the UIElement.
/// </summary>
class UIHoriGroupSizer : public UISizer
{
public:
	UIHoriGroupSizer();
	/// <summary>
	/// Padding constructor.
	/// </summary>
	/// <param name="paddingX>Keept space left and right.</param>
	/// <param name="paddingY">Keept space up and down of each UIElement.</param>
	UIHoriGroupSizer(float paddingX = 0, float paddingY = 0);
	virtual ~UIHoriGroupSizer() { }

	const bool isControllingChildren();

	void ResizeChildren(RhyM::Vec2 currentWorldScale);
	void UpdatePositionChildren();

	float paddingX = 0, paddingY = 0;

private:
	static const bool controllingChildren = true;

};
