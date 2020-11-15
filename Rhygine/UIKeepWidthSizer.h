#pragma once
#include "UISizer.h"

class UIKeepWidth : public UISizer
{
public:
	enum class VertPlacement {
		Top, Middle, Down
	};
	enum class HoriPlacement {
		Left, Middle, Right
	};

	UIKeepWidth(VertPlacement vert = VertPlacement::Down, HoriPlacement hori = HoriPlacement::Middle, float paddingX = 0.0f, float paddingY = 0.0f);

	const bool isControllingChildren();

	void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	void UpdatePositionSelf(RhyM::Rect& rect);

	VertPlacement vert;
	HoriPlacement hori;
	float paddingX, paddingY;

private:
	static const bool controllingChildren = false;

};

