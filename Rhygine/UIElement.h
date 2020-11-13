#pragma once
#include "RhyMath.h"
#include "Component.h"
#include "Drawable.h"

#include <vector>
#include <memory>

class SpriteBatch;
class UISizer;
class UIElement : public Component
{
	friend class UISizer;
public:
	~UIElement();

	virtual bool IsTouched(RhyM::Vec2 pos);
	virtual void Click() {}

	virtual void Draw(SpriteBatch* batch);
	virtual void InnerDraw(SpriteBatch* batch) = 0;

	UISizer* GetSizer();
	void SetSizer(std::unique_ptr<UISizer> newSizer);

	void SetParent(UIElement* element);
	int GetChildrenCount();
	UIElement* GetChildAt(int at);

	bool touchable = true;

protected:
	RhyM::Rect bounds;

private:
	void OnResize();
	void OnUpdatePosition();

	float width, height;
	float scaleX, scaleY;
	RhyM::Vec3 pos = RhyM::Vec3(0.0f, 0.0f, 0.0f);

	std::unique_ptr<UISizer> sizer;
	UIElement* parent;
	std::vector<UIElement*> children;
};
