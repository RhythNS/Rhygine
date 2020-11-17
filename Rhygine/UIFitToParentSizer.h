#pragma once
#include "UISizer.h"

/// <summary>
/// Copies the values of the parents bounds and applies them to the UIElements
/// bounds.
/// </summary>
class UIFitToParentSizer : public UISizer
{
public:
	virtual ~UIFitToParentSizer() { }

	const bool isControllingChildren();

	void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	void UpdatePositionSelf(RhyM::Rect& rect);

private:
	static const bool controllingChildren = false;
};
