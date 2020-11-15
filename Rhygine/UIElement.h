#pragma once
#include "RhyMath.h"
#include "RhyColor.h"
#include "Rect.h"
#include "Component.h"
#include "Drawable.h"
#include "UISizer.h"

#include <vector>
#include <memory>

class SpriteBatch;
class TextureRegion;
class Texture;
class UIElement : public Component
{
	friend class UISizer;
public:
	UIElement();
	~UIElement();
	
	void OnMouseMove(RhyM::Vec2& mousePos);
	void ClickDown();
	void ClickUp();
	virtual bool InBounds(RhyM::Vec2 pos);
	virtual void OnClickDown() {}
	virtual void OnClick() {}
	virtual void OnClickUp() {}
	virtual void OnMouseEnter() {}
	virtual void OnHover() {}
	virtual void OnMouseLeave() {}

	virtual void Draw(SpriteBatch* batch);
	virtual void InnerDraw(SpriteBatch* batch) = 0;

	UISizer* GetSizer();
	UISizer* SetSizer(std::unique_ptr<UISizer> newSizer);

	RhyM::Vec3 GetPos();
	void SetPos(RhyM::Vec3 pos);
	void SetPos(RhyM::Vec2 pos);
	void SetPos(float x, float y, float z);
	void SetPos(float x, float y);
	void SetZ(float z);
	float GetGlobalZ();

	RhyM::Vec2 GetSize();
	void SetSize(RhyM::Vec2 size);
	void SetSize(float width, float height);

	RhyM::Vec2 GetScale();
	void SetScale(RhyM::Vec2 scale);
	void SetScale(float scaleX, float scaleY);
	RhyM::Vec2 GetWorldScale();
	RhyM::Vec2 GetParentWorldScale();

	void SetPosSizeScale(float posX, float posY, float posZ, float width, float height, float scaleX, float scaleY);

	void SetParent(UIElement* element);
	int GetChildrenCount();
	UIElement* GetChildAt(int at);

	bool touchable = true;
	bool visible = true;
	RhyC::color color = RhyC::white;

protected:
	inline virtual void DrawTextureRegion(SpriteBatch* batch, TextureRegion* tex);
	inline virtual void DrawTexture(SpriteBatch* batch, Texture* tex);
	RhyM::Vec3 GetDrawPosition();
	RhyM::Rect bounds;

private:
	void OnResize(RhyM::Vec2 parentScale);
	void OnUpdatePosition();

	bool mouseInBounds = false;
	bool clicking = false;

	RhyM::Vec2 size;
	RhyM::Vec2 scale = RhyM::Vec2(1.0f, 1.0f);
	RhyM::Vec3 pos = RhyM::Vec3(0.0f, 0.0f, 20.0f);

	std::unique_ptr<UISizer> sizer;
	UIElement* parent;
	std::vector<UIElement*> children;
};
