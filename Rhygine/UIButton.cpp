#include "UIButton.h"
#include "SpriteBatch.h"


void UIButton::InnerDraw(SpriteBatch* batch)
{
	DrawTextureRegion(batch, currentTex);
}

void UIButton::OnClickDown()
{
	if (clickedTex.texture)
		currentTex = &clickedTex;
}

void UIButton::OnClickUp()
{
	if (hoverTex.texture)
		currentTex = &hoverTex;
	else
		currentTex = &normalTex;
}

void UIButton::OnMouseEnter()
{
	if (hoverTex.texture)
		currentTex = &hoverTex;
}

void UIButton::OnMouseLeave()
{
	currentTex = &normalTex;
}

void UIButton::SetTextures(TextureRegion* normal, TextureRegion* hover, TextureRegion* clicked)
{
	if (normal)
	{
		normalTex = *normal;
		if (!currentTex)
			currentTex = &normalTex;
	}
	if (hover)
		hoverTex = *hover;
	if (clicked)
		clickedTex = *clicked;
}
