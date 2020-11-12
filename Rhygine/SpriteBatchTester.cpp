#include "SpriteBatchTester.h"
#include "Gfx.h"
#include "Rhyimgui.h"

void SpriteBatchTester::Init()
{
	for (int i = 1; i < 5; i++)
	{
		texs[i - 1] = std::make_unique<Texture>(("TestModels\\Sprite\\Walk (" + std::to_string(i) + ").png").c_str(), 0);
	}

	batch = std::make_unique<SpriteBatch>();
}

void SpriteBatchTester::Draw()
{
	Gfx::GetInstance()->ClearDepth();

	static RhyM::Vec3 position = RhyM::Vec3(0.0f, 0.0f, 20.0f);
	static float width = 300.0;
	static float height = 300.0f;

	batch->Begin(&orthoCam);
	batch->Draw(texs[0].get(), 0, 0, texs[0]->GetWidth(), texs[0]->GetHeight(), position, width, height, 0, RhyC::white);
	batch->Draw(texs[0].get(), 0, 0, texs[0]->GetWidth(), texs[0]->GetHeight(), RhyM::Vec3(-200.0f, -200.0f, 20.0f), 300.0f, 300.0f, 0, RhyC::white);
	batch->End();
}
