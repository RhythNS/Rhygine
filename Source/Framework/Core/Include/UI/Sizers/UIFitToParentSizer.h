#pragma once
#include <UI/UIOwnSizer.h>

/// <summary>
/// Copies the values of the parents bounds and applies them to the UIElements
/// bounds.
/// </summary>
class UIFitToParentSizer : public UIOwnSizer
{
public:
	virtual ~UIFitToParentSizer() { }

	void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	void UpdatePositionSelf(RhyM::Rect& rect);
};
