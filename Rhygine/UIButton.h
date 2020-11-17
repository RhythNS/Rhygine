#pragma once
#include "UIElement.h"
#include "TextureRegion.h"

/// <summary>
/// A representation of a Button for a user interface. A custom button may inherit from
/// this and overwrite the OnClick method.
/// </summary>
class UIButton : public UIElement
{
public:
	virtual ~UIButton() { }
	virtual void InnerDraw(SpriteBatch* batch);
	virtual void OnClickDown();
	virtual void OnClickUp();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();

	/// <summary>
	/// Sets the textures of the UIButton. This method must be called when constructing a UIButton.
	/// </summary>
	/// <param name="normal">The texture that is displayed when the mouse is not inside the bounds of the button.</param>
	/// <param name="hover">The texture that is displayed when the mouse is inside the bounds of the button but has not yet left pressed.</param>
	/// <param name="clicked">The texture that is displayed when the mouse is inside the bounds and the left mouse button was pressed.</param>
	void SetTextures(TextureRegion* normal = nullptr, TextureRegion* hover = nullptr, TextureRegion* clicked = nullptr);

private:
	TextureRegion normalTex, hoverTex, clickedTex;
	TextureRegion* currentTex;
};

