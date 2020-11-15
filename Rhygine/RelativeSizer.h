#pragma once
#include "UISizer.h"

class RelativeSizer : public UISizer
{

public:
	const bool isControllingChildren();

	void ResizeChildren(RhyM::Vec2 currentWorldScale);
	void UpdatePositionChildren();

	RhyM::Vec2 fromToX, fromToY;
private:
	static const bool controllingChildren = true;

};

