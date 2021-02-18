#include "UIFitToParentSizer.h"
#include "UIElement.h"

void UIFitToParentSizer::ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale)
{
    RhyM::Rect parent = GetParentRect();
    rect.width = parent.width;
    rect.height = parent.height;
}

void UIFitToParentSizer::UpdatePositionSelf(RhyM::Rect& rect)
{
    RhyM::Rect parent = GetParentRect();
    rect.x = parent.x;
    rect.y = parent.y;
}
