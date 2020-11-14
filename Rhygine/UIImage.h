#pragma once
#include "UIElement.h"
#include "TextureRegion.h"

class UIImage : public UIElement
{
public:
	void InnerDraw(SpriteBatch* batch);
	TextureRegion image;
};
