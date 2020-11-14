#pragma once
#include "UIElement.h"

class UIRootElement : public UIElement
{
public:
	void Init();
	void InnerDraw(SpriteBatch* batch) {}
};
