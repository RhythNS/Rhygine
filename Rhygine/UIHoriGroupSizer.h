#pragma once
#include "UISizer.h"

class UIHoriGroupSizer : public UISizer
{
public:
	UIHoriGroupSizer();
	UIHoriGroupSizer(float paddingX = 0, float paddingY = 0);

	const bool isControllingChildren();

	void ResizeChildren(RhyM::Vec2 currentWorldScale);
	void UpdatePositionChildren();

	float paddingX = 0, paddingY = 0;

private:
	static const bool controllingChildren = true;

};
