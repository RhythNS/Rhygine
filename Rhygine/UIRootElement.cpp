#include "UIRootElement.h"
#include "UIRootSizer.h"
#include "Gameobject.h"

void UIRootElement::Init()
{
	SetSizer(std::make_unique<UIRootSizer>());
}

void UIRootElement::Delete()
{
	InnerDelete(this);
}

void UIRootElement::InnerDelete(UIElement* element)
{
	for (int i = element->children.size() - 1; i > -1; i--)
		InnerDelete(element->children[i]);
	element->GetGameObject()->RemoveComponent(element);
}
