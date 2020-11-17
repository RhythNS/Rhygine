#pragma once
#include "UISizer.h"

/// <summary>
/// Resizes and reposition its children so they form a vertical list inside the
/// bounds of the UIElement.
/// </summary>
class UIVertGroupSizer : public UISizer
{
public:
	UIVertGroupSizer();
	/// <summary>
	/// Padding constructor.
	/// </summary>
	/// <param name="paddingX>Keept space left and right of each UIElement.</param>
	/// <param name="paddingY">Keept space up and down.</param>
	UIVertGroupSizer(float paddingX = 0, float paddingY = 0);
	virtual ~UIVertGroupSizer() { }

	const bool isControllingChildren();

	void ResizeChildren(RhyM::Vec2 currentWorldScale);
	void UpdatePositionChildren();

	float paddingX = 0, paddingY = 0;

private:
	static const bool controllingChildren = true;
};
