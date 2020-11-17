#pragma once
#include "UISizer.h"

/// <summary>
/// Keeps the aspect ratio and fits the UIElement to the maximum size inside its parent.
/// </summary>
class UIKeepWidth : public UISizer
{
public:
	UIKeepWidth(VertAlignment vert = VertAlignment::Down, HoriAlignment hori = HoriAlignment::Middle, float paddingX = 0.0f, float paddingY = 0.0f);
	virtual ~UIKeepWidth() { }

	const bool isControllingChildren();

	void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	void UpdatePositionSelf(RhyM::Rect& rect);

	VertAlignment vert;
	HoriAlignment hori;
	float paddingX, paddingY;

private:
	static const bool controllingChildren = false;

};

