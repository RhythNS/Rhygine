#pragma once
#include <UI/UIElement.h>

/// <summary>
/// A bare minimum UIElement that does not draw anything.
/// </summary>
class UIContainer : public UIElement
{
public:
	virtual ~UIContainer() { }
	void InnerDraw(SpriteBatch* batch) {}
};
