#include "UIHoriGroupSizer.h"
#include "Rect.h"

UIHoriGroupSizer::UIHoriGroupSizer()
{}

UIHoriGroupSizer::UIHoriGroupSizer(float paddingX, float paddingY) : paddingX(paddingX), paddingY(paddingY)
{}

void UIHoriGroupSizer::ResizeChildren(RhyM::Vec2 currentWorldScale)
{
	int childCount = GetChildCount();
	if (childCount == 0)
		return;

	RhyM::Rect& own = GetRect();

	RhyM::Vec2 childSize = RhyM::Vec2(
		(own.width - paddingX * (childCount + 1)) / childCount,
		own.height - paddingY * 2);

	for (int i = 0; i < childCount; i++)
	{
		RhyM::Rect& childRect = GetRectChild(i);
		childRect.SetSize(childSize);
	}
}

void UIHoriGroupSizer::UpdatePositionChildren()
{
	int childCount = GetChildCount();

	if (childCount == 0)
		return;

	RhyM::Rect& own = GetRect();
	for (int i = 0; i < childCount; i++)
	{
		RhyM::Rect& childRect = GetRectChild(i);
		childRect.x = own.x + (i + 1) * paddingX + childRect.width * i;
		childRect.y = own.y + paddingY;
	}
}
