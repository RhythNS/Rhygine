#pragma once
#include "UIElement.h"

class Stage;
class UIRootElement : public UIElement
{
	friend class Stage;
public:
	void Init();
	void InnerDraw(SpriteBatch* batch) {}
private:
	void Delete();
	void InnerDelete(UIElement* element);
};
