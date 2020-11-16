#pragma once
#include "UISizer.h"

class UIFitToParentRelativeSizer : public UISizer
{
public:
	UIFitToParentRelativeSizer(float fromX, float toX, float fromY, float toY);
	const bool isControllingChildren();

	void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	void UpdatePositionSelf(RhyM::Rect& rect);

	float fromX, toX, fromY, toY;
private:
	static const bool controllingChildren = false;
};

