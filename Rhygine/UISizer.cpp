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

void UISizer::OnResize()
{
	if (!element->parent->sizer->isControllingChildren())
		ResizeSelf(element->bounds);

	if (isControllingChildren())
		ResizeChildren();
}

void UISizer::OnUpdatePosition()
{
	if (!element->parent->sizer->isControllingChildren())
		UpdatePositionSelf(element->bounds);

	if (isControllingChildren())
		UpdatePositionChildren();
}

void UISizer::ResizeSelf(RhyM::Rect& rect)
{
	rect.width = element->width * element->scaleX;
	rect.height = element->height * element->scaleY;
}

void UISizer::UpdatePositionSelf(RhyM::Rect& rect)
{
	rect.x = element->parent->bounds.x + element->pos.m_floats[0];
	rect.y = element->parent->bounds.y + element->pos.m_floats[1];

}
