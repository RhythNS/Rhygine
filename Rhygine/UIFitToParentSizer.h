#pragma once
#include "UISizer.h"

class UIFitToParentSizer : public UISizer
{
public:
	const bool isControllingChildren();

	void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	void UpdatePositionSelf(RhyM::Rect& rect);

private:
	static const bool controllingChildren = false;
};
