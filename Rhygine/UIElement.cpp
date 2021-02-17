#include "UIElement.h"
#include "Gameobject.h"
#include "UISizer.h"
#include "UILocalSizer.h"
#include "SpriteBatch.h"
#include "TextureRegion.h"
#include "Texture.h"
#include "Stage.h"

#include <cassert>

UIElement::UIElement() : sizer(std::make_unique<UILocalSizer>()), bounds(), parent(nullptr)
{
	sizer->element = this;
}

void UIElement::OnRemove()
{
	SetParent(nullptr);

	// Before removing this component, every child should have been deleted first.
	assert(children.size() == 0);
}

void UIElement::OnMouseMove(RhyM::Vec2& mousePos)
{
	if (!InBounds(mousePos)) // is mouse not in bounds?
	{
		// was the mouse in bounds in the prev frame?
		// Otherwise ignore it and dont notify the children.
		if (mouseInBounds) 
		{
			OnMouseLeave();
			mouseInBounds = false;

			// notify every child of the mouse move.
			for (auto& child : children)
			{
				child->OnMouseMove(mousePos);
			}

			// if the left mouse button was clicked then call OnClickUp without
			// calling OnClick.
			if (clicking)
			{
				OnClickUp();
				for (auto& child : children)
				{
					child->OnClickUp();
				}
			}
		}
	}
	else // mouse in bounds 
	{
		if (mouseInBounds) // was the mouse in bounds the prev frame?
		{
			OnHover();
		}
		else // mouseInBounds was prev false
		{
			OnMouseEnter();
			mouseInBounds = true;
		}

		for (auto& child : children)
		{
			child->OnMouseMove(mousePos);
		}
	}
}

void UIElement::ClickDown()
{
	if (mouseInBounds)
	{
		OnClickDown();
		for (auto& child : children)
		{
			child->ClickDown();
		}
	}
}

void UIElement::ClickUp()
{
	if (mouseInBounds)
	{
		clicking = false;
		OnClick();
		OnClickUp();
		for (auto& child : children)
		{
			child->ClickUp();
		}
	}
}

bool UIElement::InBounds(RhyM::Vec2 pos)
{
	return touchable && visible && bounds.Overlaps(pos);
}

void UIElement::Draw(SpriteBatch* batch)
{
	if (visible) // TODO: Consider changing this, so that children are also not displayed
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

UISizer* UIElement::SetSizer(std::unique_ptr<UISizer> newSizer)
{
	newSizer->element = this;
	sizer.reset();
	sizer.swap(newSizer);
	OnResize(GetParentWorldScale());
	OnUpdatePosition();
	return sizer.get();
}

RhyM::Vec3 UIElement::GetPos()
{
	return RhyM::Vec3(bounds.x, bounds.y, pos.z);
}

void UIElement::SetPos(RhyM::Vec3 _pos)
{
	pos = _pos;
	OnUpdatePosition();
}

void UIElement::SetPos(RhyM::Vec2 _pos)
{
	pos.x = _pos.x;
	pos.y = _pos.y;
	OnUpdatePosition();
}

void UIElement::SetPos(float x, float y, float z)
{
	pos.Set(x, y, z);
	OnUpdatePosition();
}

void UIElement::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
	OnUpdatePosition();
}

void UIElement::SetZ(float z)
{
	pos.z = z;
}

float UIElement::GetGlobalZ()
{
	return parent == nullptr ? pos.z : parent->GetGlobalZ() + pos.z;
}

RhyM::Vec2 UIElement::GetSize()
{
	return size;
}

void UIElement::SetSize(RhyM::Vec2 _size)
{
	size = _size;
	OnResize(GetParentWorldScale());
}

void UIElement::SetSize(float width, float height)
{
	size.Set(width, height);
	OnResize(GetParentWorldScale());
}

RhyM::Vec2 UIElement::GetScale()
{
	return size;
}

void UIElement::SetScale(RhyM::Vec2 _scale)
{
	scale = _scale;
	OnResize(GetParentWorldScale());
}

void UIElement::SetScale(float scaleX, float scaleY)
{
	scale.Set(scaleX, scaleY);
	OnResize(GetParentWorldScale());
}

inline void UIElement::DrawTextureRegion(SpriteBatch* batch, TextureRegion* tex)
{
	batch->Draw(tex, bounds.x, bounds.y, GetGlobalZ(), bounds.width, bounds.height, 0, color);
}

inline void UIElement::DrawTexture(SpriteBatch* batch, Texture* tex)
{
	batch->Draw(tex, 0.0f, 0.0f, static_cast<float>(tex->GetWidth()), static_cast<float>(tex->GetHeight()), RhyM::Vec3(bounds.x, bounds.y, GetGlobalZ()), bounds.width, bounds.height, 0.0f, color);
}

RhyM::Vec3 UIElement::GetDrawPosition()
{
	return RhyM::Vec3(bounds.x, bounds.y, GetGlobalZ());
}

void UIElement::OnResize(RhyM::Vec2 parentScale)
{
	parentScale.Scl(scale);
	sizer->OnResize(parentScale);
	for (auto& child : children)
	{
		child->OnResize(parentScale);
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

RhyM::Vec2 UIElement::GetWorldScale()
{
	return parent == nullptr ? scale : RhyM::Vec2::Scale(parent->GetWorldScale(), scale);
}

RhyM::Vec2 UIElement::GetParentWorldScale()
{
	return parent == nullptr ? RhyM::Vec2(1.0f, 1.0f) : parent->GetWorldScale();
}

void UIElement::SetPosSizeScale(float posX, float posY, float posZ, float width, float height, float scaleX, float scaleY)
{
	pos.Set(posX, posY, posZ);
	size.Set(width, height);
	scale.Set(scaleX, scaleY);
	OnResize(GetParentWorldScale());
	OnUpdatePosition();
}

void UIElement::SetParent(UIElement* element)
{
	// new parent is the same
	if (element == parent)
		return;

	// If the old parent is not null then remove the reference to this transfrom.
	if (parent != nullptr)
		std::erase(parent->children, this);

	// If the new parent is not null then add it a reference to it.
	if (element != nullptr)
		element->children.push_back(this);

	parent = element;

	// parent is controlling us?
	if (parent != nullptr && parent->GetSizer()->isControllingChildren())
	{
		parent->OnResize(parent->GetParentWorldScale());
		parent->OnUpdatePosition();
	}
	else // parent is not controlling us
	{
		OnResize(GetParentWorldScale());
		OnUpdatePosition();
	}
}

int UIElement::GetChildrenCount()
{
	return static_cast<int>(children.size());
}

UIElement* UIElement::GetChildAt(int at)
{
	assert(at < children.size() && at > -1);
	return children[at];
}
