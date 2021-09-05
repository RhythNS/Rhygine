#include <UI/Sizers/UIRootSizer.h>
#include <Core/Window.h>

void UIRootSizer::ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale)
{
	rect.width = static_cast<float>(Window::GetInstance()->GetWidth());
	rect.height = static_cast<float>(Window::GetInstance()->GetHeight());
}

void UIRootSizer::UpdatePositionSelf(RhyM::Rect& rect)
{
	rect.x = 0;
	rect.y = 0;
}
