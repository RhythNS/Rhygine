#pragma once
#include "UISizer.h"

/// <summary>
/// Special sizer for root element.
/// </summary>
class UIRootSizer : public UISizer
{
public:
	virtual ~UIRootSizer() { }

	const bool isControllingChildren();

	void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	void UpdatePositionSelf(RhyM::Rect& rect);

private:
	static const bool controllingChildren = false;
};
