#pragma once
#include <UI/UIOwnSizer.h>

/// <summary>
/// Fits the UIElement to the bounds of its parent relativly based on the from and to values.
/// From and To values are in percentage(0.0f-1.0f).
/// If the parents bounds would be x=0, y=0, width=100, height=100
/// and the this UISizers from and to values fromX=0.2f, toX=0.8, fromY=0.0f, toY=0.5
/// then the sizer would resize the bounds to width=0.6, height=0.5 at the
/// position x=20, y=0.
/// </summary>
class UIFitToParentRelativeSizer : public UIOwnSizer
{
public:
	UIFitToParentRelativeSizer(float fromX, float toX, float fromY, float toY);
	virtual ~UIFitToParentRelativeSizer() { }

	const bool isControllingChildren();

	void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	void UpdatePositionSelf(RhyM::Rect& rect);

	float fromX, toX, fromY, toY;
private:
	static const bool controllingChildren = false;
};
