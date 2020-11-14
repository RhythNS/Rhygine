#pragma once
#include "RhyMath.h"
#include "Rect.h"

class UIElement;
class UISizer
{
	friend class UIElement;
protected:
	void OnResize(RhyM::Vec2 currentWorldScale);
	void OnUpdatePosition();
	
	virtual const bool isControllingChildren() = 0;
	
	virtual void ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale);
	virtual void UpdatePositionSelf(RhyM::Rect& rect);
	
	virtual void ResizeChildren(RhyM::Vec2 currentWorldScale) {}
	virtual void UpdatePositionChildren() {}

	int GetChildCount();
	RhyM::Rect& GetRect();
	RhyM::Rect& GetRectChild(int at);
	RhyM::Rect GetParentRect();

	UIElement* element;
};
