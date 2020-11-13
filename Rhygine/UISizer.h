#pragma once
#include "RhyMath.h"

class UIElement;
class UISizer
{
	friend class UIElement;
protected:
	void OnResize();
	void OnUpdatePosition();
	
	virtual bool isControllingChildren() = 0;
	
	virtual void ResizeSelf(RhyM::Rect& rect);
	virtual void UpdatePositionSelf(RhyM::Rect& rect);
	
	virtual void ResizeChildren() {}
	virtual void UpdatePositionChildren() {}

	int GetChildCount();
	RhyM::Rect& GetRect();
	RhyM::Rect& GetRectChild(int at);

	UIElement* element;
};
