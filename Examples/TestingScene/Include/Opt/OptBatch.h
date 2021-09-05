#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>
#include <ECS/Drawable.h>
#include <ECS/Bindable/Texture.h>
#include <2D/TextureRegion.h>

#include <memory>

/// <summary>
/// Tests to see the frame time of two images drawn to the SpriteBatch.
/// </summary>
class OptBatch : public Component, public Updatable, public Drawable 
{
public:
	void Init();
	void Update();
	void Draw();
private:
	std::unique_ptr<Texture> tex1, tex2;
	TextureRegion reg1, reg2;
};
