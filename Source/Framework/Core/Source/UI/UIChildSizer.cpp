#include <UI/UIChildSizer.h>
#include <UI/UIElement.h>

int UIChildSizer::GetChildCount()
{
	return static_cast<int>(element->children.size());
}

RhyM::Rect& UIChildSizer::GetRect()
{
	return element->bounds;
}

RhyM::Rect& UIChildSizer::GetRectChild(int at)
{
	return element->children[at]->bounds;
}

RhyM::Rect UIChildSizer::GetParentRect()
{
	if (element->parent == nullptr)
	{
		RhyM::Rect tempParent = RhyM::Rect(0.0f, 0.0f, 1.0f, 1.0f);
		return tempParent;
	}
	return  element->parent->bounds;
}
