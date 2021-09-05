#include <UI/Sizers/UIVertGroupSizer.h>

UIVertGroupSizer::UIVertGroupSizer()
{}

UIVertGroupSizer::UIVertGroupSizer(float paddingX, float paddingY) : paddingX(paddingX), paddingY(paddingY)
{}

void UIVertGroupSizer::ResizeChildren(RhyM::Vec2 currentWorldScale)
{
	int childCount = GetChildCount();
	if (childCount == 0)
		return;

	RhyM::Rect& own = GetRect();

	RhyM::Vec2 childSize = RhyM::Vec2(
		own.width - paddingX * 2,
		(own.height - paddingY * (childCount + 1)) / childCount);

	for (int i = 0; i < childCount; i++)
	{
		RhyM::Rect& childRect = GetRectChild(i);
		childRect.SetSize(childSize);
	}
}

void UIVertGroupSizer::UpdatePositionChildren()
{
	int childCount = GetChildCount();

	if (childCount == 0)
		return;

	RhyM::Rect& own = GetRect();
	for (int i = 0; i < childCount; i++)
	{
		RhyM::Rect& childRect = GetRectChild(i);
		childRect.x = own.x + paddingX;
		childRect.y = own.y + (i + 1) * paddingY + childRect.height * i;
	}
}
