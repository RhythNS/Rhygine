#include "UIElement.h"
#include "Gameobject.h"
#include "UISizer.h"
#include "UILocalSizer.h"
#include "SpriteBatch.h"
#include "TextureRegion.h"

#include <cassert>

UIElement::UIElement() : sizer(std::make_unique<UILocalSizer>()), bounds(), parent(nullptr)
{
	sizer->element = this;
}

UIElement::~UIElement()
{
	if (parent != nullptr)
		SetParent(nullptr);
}

void UIElement::OnMouseMove(RhyM::Vec2& mousePos)
{
	if (!InBounds(mousePos))
	{
		if (mouseInBounds)
		{
			OnMouseLeave();
			mouseInBounds = false;

			for (auto& child : children)
			{
				child->OnMouseMove(mousePos);
			}

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
		if (mouseInBounds)
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
	if (visible)
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
	return sizer.get();
}

RhyM::Vec3 UIElement::GetPos()
{
	return RhyM::Vec3(bounds.x, bounds.y, pos.m_floats[2]);
}

void UIElement::SetPos(RhyM::Vec3 _pos)
{
	pos = _pos;
	OnUpdatePosition();
}

void UIElement::SetPos(RhyM::Vec2 _pos)
{
	pos.m_floats[0] = _pos.x;
	pos.m_floats[1] = _pos.y;
	OnUpdatePosition();
}

void UIElement::SetPos(float x, float y, float z)
{
	pos.setValue(x, y, z);
	OnUpdatePosition();
}

void UIElement::SetPos(float x, float y)
{
	pos.m_floats[0] = x;
	pos.m_floats[1] = y;
	OnUpdatePosition();
}

void UIElement::SetZ(float z)
{
	pos.m_floats[2] = z;
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
	batch->Draw(tex, bounds.x, bounds.y, pos.m_floats[2], bounds.width, bounds.height, 0, color);
}

RhyM::Vec3 UIElement::GetDrawPosition()
{
	return RhyM::Vec3(bounds.x, bounds.y, pos.m_floats[2]);
}

void UIElement::OnResize(RhyM::Vec2 parentScale)
{
	parentScale.SymMul(scale);
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
	return parent == nullptr ? scale : parent->GetWorldScale().SymMul(scale);
}

RhyM::Vec2 UIElement::GetParentWorldScale()
{
	return parent == nullptr ? RhyM::Vec2(1.0f, 1.0f) : parent->GetWorldScale();
}

void UIElement::SetPosSizeScale(float posX, float posY, float posZ, float width, float height, float scaleX, float scaleY)
{
	pos.setValue(posX, posY, posZ);
	size.Set(width, height);
	scale.Set(scaleX, scaleY);
	OnResize(GetParentWorldScale());
	OnUpdatePosition();
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

	parent = element;

	if (parent != nullptr && parent->GetSizer()->isControllingChildren())
	{
		parent->OnResize(parent->GetParentWorldScale());
		parent->OnUpdatePosition();
	}
	else
	{
		OnResize(GetParentWorldScale());
		OnUpdatePosition();
	}
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
