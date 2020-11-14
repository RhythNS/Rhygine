#include "UIRootSizer.h"
#include "Window.h"

const bool UIRootSizer::isControllingChildren()
{
	return controllingChildren;
}

void UIRootSizer::ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale)
{
	rect.width = Window::GetInstance()->GetWidth();
	rect.height = Window::GetInstance()->GetHeight();
}

void UIRootSizer::UpdatePositionSelf(RhyM::Rect& rect)
{
	rect.x = 0;
	rect.y = 0;
}
