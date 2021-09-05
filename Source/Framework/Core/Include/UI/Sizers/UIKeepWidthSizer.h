#pragma once
#include <UI/UIOwnSizer.h>
#include <UI/UIElement.h>

/// <summary>
/// Keeps the aspect ratio and fits the UIElement to the maximum size inside its parent.
/// </summary>
class UIKeepWidth : public UIOwnSizer
{
public:
	UIKeepWidth(VertAlignment vert = VertAlignment::Down, HoriAlignment hori = HoriAlignment::Middle, float paddingX = 0.0f, float paddingY = 0.0f);
	virtual ~UIKeepWidth() { }

	void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	void UpdatePositionSelf(RhyM::Rect& rect);

	VertAlignment vert;
	HoriAlignment hori;
	float paddingX, paddingY;
};
