#include "UIElement.h"
#include "Gameobject.h"
#include "UISizer.h"
#include <cassert>

UIElement::~UIElement()
{
	if (parent != nullptr)
		SetParent(nullptr);
}

bool UIElement::IsTouched(RhyM::Vec2 pos)
{
	return touchable && bounds.Overlaps(pos);
}

void UIElement::Draw(SpriteBatch* batch)
{
	InnerDraw(batch);
	for (auto& child : children)
	{
		child->Draw(batch);
	}
}

UISizer* UIElement::GetSizer()
{
	return sizer.get();
}

void UIElement::SetSizer(std::unique_ptr<UISizer> newSizer)
{
	newSizer->element = this;
	newSizer.reset();
	sizer.swap(newSizer);
	OnResize();
}

void UIElement::OnResize()
{
	sizer->OnResize();
	for (auto& child : children)
	{
		child->OnResize();
	}
}

void UIElement::OnUpdatePosition()
{
	sizer->OnUpdatePosition();
	for (auto& child : children)
	{
		child->OnUpdatePosition();
	}
}

void UIElement::SetParent(UIElement* element)
{
	if (element == parent)
		return;

	// If the old parent is not null then remove the reference to this transfrom.
	if (parent != nullptr)
		std::erase(parent->children, this);

	// If the new parent is not null then add it a reference to it.
	if (element != nullptr)
		element->children.push_back(this);

	OnResize();
	OnUpdatePosition();
}

int UIElement::GetChildrenCount()
{
	return children.size();
}

UIElement* UIElement::GetChildAt(int at)
{
	assert(at < children.size() && at > -1);
	return children[at];
}
