#include "UIVertPercentageSizer.h"
#include "RhyWin.h"

UIVertPercentageSizer::UIVertPercentageSizer()
{
}

UIVertPercentageSizer::UIVertPercentageSizer(std::vector<float> percentages) : percentages(percentages)
{
}

const bool UIVertPercentageSizer::isControllingChildren()
{
	return controllingChildren;
}

void UIVertPercentageSizer::ResizeChildren(RhyM::Vec2 currentWorldScale)
{
	RhyM::Rect rect = GetRect();
	float atPercentage = 0.0f;
	for (int i = 0; i < GetChildCount(); i++)
	{
		float newPercentage = 0.0f;
		if (i < percentages.size())
			newPercentage = percentages[i];
		
		RhyM::Rect& child = GetRectChild(i);
		child.width = rect.width;
		child.height = rect.height * newPercentage;

		atPercentage += newPercentage;
	}

}

void UIVertPercentageSizer::UpdatePositionChildren()
{
	RhyM::Rect rect = GetRect();
	float atY = rect.y;
	for (int i = 0; i < GetChildCount(); i++)
	{
		RhyM::Rect& child = GetRectChild(i);
		child.x = rect.x;

		child.y = atY;
		if (rect.height != 0)
			atY += rect.height * (child.height / rect.height);
	}
}
