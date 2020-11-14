#include "UIRootElement.h"
#include "UIRootSizer.h"

void UIRootElement::Init()
{
	SetSizer(std::make_unique<UIRootSizer>());
}
