#include "UISizer.h"
#include "UIElement.h"
#include "Rect.h"

#include <cassert>

int UISizer::GetChildCount()
{
	return static_cast<int>(element->children.size());
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
	if (element->parent == nullptr)
	{
		RhyM::Rect tempParent = RhyM::Rect(0.0f, 0.0f, 1.0f, 1.0f);
		return tempParent;
	}
	return  element->parent->bounds;
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
	if (element->parent == nullptr || !element->parent->sizer->isControllingChildren())
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
	RhyM::Rect parentRect = GetParentRect();
	rect.x = parentRect.x + element->pos.x;
	rect.y = parentRect.y + element->pos.y;
}

void UISizer::UpdatePositionDefaultHoriVert(RhyM::Rect& rect, VertAlignment vert, HoriAlignment hori, float paddingX, float paddingY)
{
	RhyM::Rect parentRect = GetParentRect();
	switch (vert)
	{
	case VertAlignment::Up:
		rect.y = parentRect.y + parentRect.height - rect.height - paddingY;
		break;
	case VertAlignment::Middle:
		rect.y = parentRect.y + parentRect.height / 2 - rect.height / 2;
		break;
	case VertAlignment::Down:
		rect.y = parentRect.y + paddingY;
		break;
	default:
		assert(false);
		break;
	}
	switch (hori)
	{
	case HoriAlignment::Left:
		rect.x = parentRect.x + paddingX;
		break;
	case HoriAlignment::Middle:
		rect.x = parentRect.x + parentRect.width / 2 - rect.width / 2;
		break;
	case HoriAlignment::Right:
		rect.x = parentRect.x + parentRect.width - rect.width - paddingX;
		break;
	default:
		assert(false);
		break;
	}
}
