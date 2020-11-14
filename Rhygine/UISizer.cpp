#include "UISizer.h"
#include "UIElement.h"

int UISizer::GetChildCount()
{
	return element->children.size();
}

RhyM::Rect& UISizer::GetRect()
{
	return element->bounds;
}

RhyM::Rect& UISizer::GetRectChild(int at)
{
	return element->children[at]->bounds;
}

RhyM::Rect UISizer::GetParentRect()
{
	return element->parent->bounds;
}

void UISizer::OnResize(RhyM::Vec2 currentWorldScale)
{
	if (element->parent == nullptr || !element->parent->sizer->isControllingChildren())
		ResizeSelf(element->bounds, currentWorldScale);

	if (isControllingChildren())
		ResizeChildren(currentWorldScale);
}

void UISizer::OnUpdatePosition()
{
	if (!element->parent->sizer->isControllingChildren())
		UpdatePositionSelf(element->bounds);

	if (isControllingChildren())
		UpdatePositionChildren();
}

void UISizer::ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale)
{
	rect.width = element->size.x * currentWorldScale.x;
	rect.height = element->size.y * currentWorldScale.y;
}

void UISizer::UpdatePositionSelf(RhyM::Rect& rect)
{
	rect.x = element->parent->bounds.x + element->pos.m_floats[0];
	rect.y = element->parent->bounds.y + element->pos.m_floats[1];
}
