#pragma once
#include <ECS/Component.h>
#include <ECS/Drawable.h>
#include <ECS/Bindable/Texture.h>
#include <2D/SpriteBatch.h>
#include <2D/OrthographicCamera.h>

#include <memory>

/// <summary>
/// Simple test to draw a texture to the stages spritebatch.
/// </summary>
class SpriteBatchTester : public Component, public Drawable
{
public:
	void Init();
	void Draw();
private:
	std::unique_ptr<Texture> texs[4];
};
