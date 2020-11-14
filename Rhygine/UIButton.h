#pragma once
#include "UIElement.h"
#include "TextureRegion.h"

class UIButton : public UIElement
{
public:
	virtual void InnerDraw(SpriteBatch* batch);
	virtual void OnClickDown();
	virtual void OnClickUp();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();

	void SetTextures(TextureRegion* normal = nullptr, TextureRegion* hover = nullptr, TextureRegion* clicked = nullptr);

private:
	TextureRegion normalTex, hoverTex, clickedTex;
	TextureRegion* currentTex;
};

