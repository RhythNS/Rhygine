#pragma once
#include "UIElement.h"
#include "MonoFont.h"

/// <summary>
/// Renders a Monofont text to its bounds.
/// </summary>
class UIFont : public UIElement
{
public:
	enum class VertAlignment {
		Up, Middle, Down
	};
	enum class HoriAlignment {
		Left, Middle, Right
	};
	virtual ~UIFont() { }

	virtual void InnerDraw(SpriteBatch* batch);
	/// <summary>
	/// Sets all needed references.
	/// </summary>
	/// <param name="font">A reference to the monofont.</param>
	/// <param name="wordWrap">Wheter words should render to the next line when they go over the width of the bounds.</param>
	/// <param name="vert">Vertical alignment of the font to the bounds of the UIElement.</param>
	/// <param name="hori">Horizontal alignment of the font to the bounds of the UIElement.</param>
	/// <param name="textScale">Multiplies this value with the default width of the monofont when drawing.</param>
	virtual void Set(MonoFont* font, bool wordWrap = false, VertAlignment vert = VertAlignment::Up, HoriAlignment hori = HoriAlignment::Left, float textScale = 1.0f);

	std::string text;
	VertAlignment vert = VertAlignment::Up;
	HoriAlignment hori = HoriAlignment::Left;
	bool wordWrap = false;
	float textScale;
private:
	MonoFont* font;
};
