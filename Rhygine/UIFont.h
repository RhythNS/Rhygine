#pragma once
#include "UIElement.h"
#include "MonoFont.h"

class UIFont : public UIElement
{
public:
	enum class VertAlignment {
		Up, Middle, Down
	};
	enum class HoriAlignment {
		Left, Middle, Right
	};

	void InnerDraw(SpriteBatch* batch);
	void Set(MonoFont* font, bool wordWrap = false, VertAlignment vert = VertAlignment::Up, HoriAlignment hori = HoriAlignment::Left, float textScale = 1.0f);

	std::string text;
	VertAlignment vert = VertAlignment::Up;
	HoriAlignment hori = HoriAlignment::Left;
	bool wordWrap = false;
	float textScale;
private:
	MonoFont* font;
};
