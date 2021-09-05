#pragma once
#include <UI/UIOwnSizer.h>

/// <summary>
/// Special sizer for root element.
/// </summary>
class UIRootSizer : public UIOwnSizer
{
public:
	virtual ~UIRootSizer() { }

	void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	void UpdatePositionSelf(RhyM::Rect& rect);
};
