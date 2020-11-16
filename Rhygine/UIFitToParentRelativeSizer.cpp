#include "UIFitToParentRelativeSizer.h"

#include <cassert>

UIFitToParentRelativeSizer::UIFitToParentRelativeSizer(float fromX, float toX, float fromY, float toY)
    : fromX(fromX), toX(toX), fromY(fromY), toY(toY)
{
    assert(fromX < toX&& fromX >= 0.0f && toY <= 1.0f &&
        fromY < toY&& fromY >= 0.0f && toY <= 1.0f);
}

const bool UIFitToParentRelativeSizer::isControllingChildren()
{
    return controllingChildren;
}

void UIFitToParentRelativeSizer::ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale)
{
    RhyM::Rect parentRect = GetParentRect();
    rect.width = (toX - fromX) * parentRect.width;
    rect.height = (toY - fromY) * parentRect.height;
}

void UIFitToParentRelativeSizer::UpdatePositionSelf(RhyM::Rect& rect)
{
    RhyM::Rect parentRect = GetParentRect();
    rect.x = parentRect.x + parentRect.width * fromX;
    rect.y = parentRect.y + parentRect.height * fromY;
}
