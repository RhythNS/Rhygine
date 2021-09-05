#include <UI/Elements/UIRootElement.h>
#include <UI/Sizers/UIRootSizer.h>
#include <ECS/GameObject.h>

void UIRootElement::Init()
{
	SetOwnSizer(std::make_unique<UIRootSizer>());
}

void UIRootElement::Delete()
{
	InnerDelete(this);
}

void UIRootElement::InnerDelete(UIElement* element)
{
	for (int i = static_cast<int>(element->children.size()) - 1; i > -1; i--)
		InnerDelete(element->children[i]);
	element->GetGameObject()->RemoveComponent(element);
}
