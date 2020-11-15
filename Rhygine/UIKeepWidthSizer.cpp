#include "UIKeepWidthSizer.h"
#include "UIElement.h"
#include <cassert>

UIKeepWidth::UIKeepWidth(VertPlacement vert, HoriPlacement hori, float paddingX, float paddingY) :
	vert(vert), hori(hori), paddingX(paddingX), paddingY(paddingY)
{
}

const bool UIKeepWidth::isControllingChildren()
{
	return controllingChildren;
}

void UIKeepWidth::ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale)
{
	RhyM::Vec2 size = element->GetSize();
	float scale =
		std::min(1.0f,
			std::min(
				rect.width / (currentWorldScale.x * (size.x - paddingX)),
				rect.height / (currentWorldScale.y * (size.y - paddingY)
					)
			)
		);

	rect.width = currentWorldScale.x * size.x * scale;
	rect.height = currentWorldScale.y * size.y * scale;
}

void UIKeepWidth::UpdatePositionSelf(RhyM::Rect& rect)
{
	RhyM::Rect parentRect = GetParentRect();
	switch (vert)
	{
	case UIKeepWidth::VertPlacement::Top:
		rect.y = parentRect.y + parentRect.height - rect.height - paddingY;
		break;
	case UIKeepWidth::VertPlacement::Middle:
		rect.y = parentRect.y + parentRect.height / 2 - rect.height / 2 - paddingY;
		break;
	case UIKeepWidth::VertPlacement::Down:
		rect.y = parentRect.y + paddingY;
		break;
	default:
		assert(false);
		break;
	}
	switch (hori)
	{
	case UIKeepWidth::HoriPlacement::Left:
		rect.x = parentRect.x + paddingX;
		break;
	case UIKeepWidth::HoriPlacement::Middle:
		rect.x = parentRect.x + parentRect.width / 2 - rect.width / 2 - paddingX;
		break;
	case UIKeepWidth::HoriPlacement::Right:
		rect.x = parentRect.x + parentRect.width - rect.width - paddingX;
		break;
	default:
		assert(false);
		break;
	}
}
