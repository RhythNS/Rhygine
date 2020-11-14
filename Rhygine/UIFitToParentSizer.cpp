#include "UIFitToParentSizer.h"
#include "UIElement.h"

const bool UIFitToParentSizer::isControllingChildren()
{
    return controllingChildren;
}

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
