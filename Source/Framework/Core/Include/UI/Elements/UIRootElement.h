#pragma once
#include <UI/UIElement.h>

class Stage;
/// <summary>
/// The root element that the stage has.
/// </summary>
class UIRootElement : public UIElement
{
	friend class Stage;
public:
	virtual ~UIRootElement() { }

	void Init();
	void InnerDraw(SpriteBatch* batch) {}
private:
	void Delete();
	void InnerDelete(UIElement* element);
};
