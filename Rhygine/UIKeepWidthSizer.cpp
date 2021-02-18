#include "UIKeepWidthSizer.h"
#include "UIElement.h"
#include <cassert>

UIKeepWidth::UIKeepWidth(VertAlignment vert, HoriAlignment hori, float paddingX, float paddingY) :
	vert(vert), hori(hori), paddingX(paddingX), paddingY(paddingY)
{
}

void UIKeepWidth::ResizeSelf(RhyM::Rect& rect, RhyM::Vec2 currentWorldScale)
{
	RhyM::Vec2 size = element->GetSize();
	RhyM::Rect parentRect = GetParentRect();
	float scale =
	//	std::min(1.0f,
			std::min(
				parentRect.width / (currentWorldScale.x * (size.x + paddingX)),
				parentRect.height / (currentWorldScale.y * (size.y + paddingY)
					)
	//		)
		);

	rect.width = currentWorldScale.x * size.x * scale;
	rect.height = currentWorldScale.y * size.y * scale;
}

void UIKeepWidth::UpdatePositionSelf(RhyM::Rect& rect)
{
	UpdatePositionDefaultHoriVert(rect, vert, hori, paddingX, paddingY);
}
