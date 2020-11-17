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
class UIRootElement;
/// <summary>
/// UI Elements are used for creating user interface elements like a textbox,
/// a button or an image. Custom elements should inherit from this class and overwrite
/// any of the virtual methods to archieve custom behaviours.
/// A UIElement has a UISizer which resizes and positions the bounds of the UIElement.
/// </summary>
class UIElement : public Component
{
	friend class UISizer;
	friend class UIRootElement;
public:
	UIElement();
	virtual ~UIElement() {}
	void OnRemove();
	
	/// <summary>
	/// Should be called when the mouse moves inside the window.
	/// </summary>
	/// <param name="mousePos">The current mouse position.</param>
	void OnMouseMove(RhyM::Vec2& mousePos);
	/// <summary>
	/// Should be called when the left mouse button was clicked.
	/// This method calls the same method on its children the mouse click was in
	/// their rectangle bounds.
	/// </summary>
	void ClickDown();
	/// <summary>
	/// See Click Down.
	/// </summary>
	void ClickUp();
	/// <summary>
	/// Check if a position is inside the UIElement. Should be overwritten when the
	/// UIElement changes the way how the bounds rectangle works.
	/// </summary>
	/// <param name="pos">The position that should be checked if it is inside the element.</param>
	virtual bool InBounds(RhyM::Vec2 pos);
	/// <summary>
	/// Called when the mouse has clicked inside the bounds of this UIElement but was not
	/// yet released.
	/// </summary>
	virtual void OnClickDown() {}
	/// <summary>
	/// Called when the mouse clicked and was released inside the bounds of this UIElement.
	/// </summary>
	virtual void OnClick() {}
	/// <summary>
	/// Called when the mouse released or has left the bounds of this UIElement.
	/// </summary>
	virtual void OnClickUp() {}
	/// <summary>
	/// Called when the mouse enters the bounds of this UIElement.
	/// </summary>
	virtual void OnMouseEnter() {}
	/// <summary>
	/// Called every frame when the mouse was inside thie bounds of this UIElement.
	/// </summary>
	virtual void OnHover() {}
	/// <summary>
	/// Called when the mouse has left the bounds of this UIElement.
	/// </summary>
	virtual void OnMouseLeave() {}

	/// <summary>
	/// Draws the UIElement to the given SpriteBatch.
	/// </summary>
	virtual void Draw(SpriteBatch* batch);
	/// <summary>
	/// Should draw the UIElement to the bounds of the UIElement.
	/// </summary>
	virtual void InnerDraw(SpriteBatch* batch) = 0;

	/// <summary>
	/// Gets a reference to the current sizer.
	/// </summary>
	UISizer* GetSizer();
	/// <summary>
	/// Sets the current sizer.
	/// </summary>
	/// <returns>A reference to the current sizer for chaining.</returns>
	UISizer* SetSizer(std::unique_ptr<UISizer> newSizer);

	RhyM::Vec3 GetPos();
	void SetPos(RhyM::Vec3 pos);
	void SetPos(RhyM::Vec2 pos);
	void SetPos(float x, float y, float z);
	void SetPos(float x, float y);
	void SetZ(float z);
	/// <summary>
	/// Recursivly multiply the z value with z values of all parents of the tree.
	/// </summary>
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

	/// <summary>
	/// Sets the new parent of the UIElement. This method invokes resize and reposition on its parent or itself if
	/// the new parent was null.
	/// </summary>
	/// <param name="element">The new parent.</param>
	void SetParent(UIElement* element);
	/// <summary>
	/// Gets the amount of children of this UIElement.
	/// </summary>
	int GetChildrenCount();
	/// <summary>
	/// Gets the child at a given position.
	/// </summary>
	UIElement* GetChildAt(int at);

	bool touchable = true;
	bool visible = true;
	RhyC::color color = RhyC::white;

protected:
	/// <summary>
	/// Helper method for drawing a texture region to the bounds of this UIElement.
	/// </summary>
	inline virtual void DrawTextureRegion(SpriteBatch* batch, TextureRegion* tex);
	/// <summary>
	/// Helper method for frawing a texture to the bounds of this UIElement.
	/// </summary>
	inline virtual void DrawTexture(SpriteBatch* batch, Texture* tex);
	/// <summary>
	/// Gets the position and z value of this UIElement.
	/// </summary>
	RhyM::Vec3 GetDrawPosition();
	RhyM::Rect bounds;

private:
	/// <summary>
	/// Called when this element should resize itself.
	/// </summary>
	/// <param name="parentScale">The scale of the parent.</param>
	void OnResize(RhyM::Vec2 parentScale);
	/// <summary>
	/// Called when this element should reposition itself.
	/// </summary>
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
