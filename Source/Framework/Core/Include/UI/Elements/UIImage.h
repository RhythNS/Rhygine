#pragma once
#include <UI/UIElement.h>
#include <2D/TextureRegion.h>

/// <summary>
/// Draws a texture region to the its bounds when drawn.
/// </summary>
class UIImage : public UIElement
{
public:
	virtual ~UIImage() { }

	void InnerDraw(SpriteBatch* batch);
	TextureRegion image;
};
