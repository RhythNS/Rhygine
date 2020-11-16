#pragma once
#include "UISizer.h"

class UIKeepWidth : public UISizer
{
public:
	UIKeepWidth(VertAlignment vert = VertAlignment::Down, HoriAlignment hori = HoriAlignment::Middle, float paddingX = 0.0f, float paddingY = 0.0f);

	const bool isControllingChildren();

	void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	void UpdatePositionSelf(RhyM::Rect& rect);

	VertAlignment vert;
	HoriAlignment hori;
	float paddingX, paddingY;

private:
	static const bool controllingChildren = false;

};

