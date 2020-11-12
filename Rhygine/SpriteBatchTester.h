#pragma once
#include "Component.h"
#include "Drawable.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "OrthographicCamera.h"

#include <memory>

class SpriteBatchTester : public Component, public Drawable
{
public:
	void Init();
	void Draw();
private:
	std::unique_ptr<SpriteBatch> batch;
	std::unique_ptr<Texture> texs[4];
	OrthographicCamera orthoCam{RhyM::Vec3(0.0f,0.0f,0.0f)};
};