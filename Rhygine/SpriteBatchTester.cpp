#include "SpriteBatchTester.h"
#include "Gfx.h"
#include "Rhyimgui.h"

void SpriteBatchTester::Init()
{
	for (int i = 1; i < 5; i++)
	{
		texs[i - 1] = std::make_unique<Texture>(("TestModels\\Sprite\\Walk (" + std::to_string(i) + ").png").c_str(), 0);
	}
}

void SpriteBatchTester::Draw()
{
	GetSpriteBatch()->Draw(texs[0].get(), 0.0f, 0.0f, static_cast<float>(texs[0]->GetWidth()), static_cast<float>(texs[0]->GetHeight()), RhyM::Vec3(0.0f, 0.0f, 20.0f), 300.0f, 300.0f, 0.0f, RhyC::white);
	GetSpriteBatch()->Draw(texs[0].get(), 0.0f, 0.0f, static_cast<float>(texs[0]->GetWidth()), static_cast<float>(texs[0]->GetHeight()), RhyM::Vec3(-100.0f, -100.0f, 20.0f), 300.0f, 300.0f, 0.0f, RhyC::white);
}
