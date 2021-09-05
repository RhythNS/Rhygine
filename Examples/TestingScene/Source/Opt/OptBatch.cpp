#include <Opt/OptBatch.h>
#include <ECS/GameObject.h>
#include <ECS/Stage.h>
#include <2D/SpriteBatch.h>

void OptBatch::Init()
{
	tex1 = std::make_unique<Texture>("TestModels\\Sprite\\Walk (1).png", 0);
	reg1 = TextureRegion(tex1.get());
	tex2 = std::make_unique<Texture>("TestModels\\Sprite\\Walk (2).png", 0);
	reg2 = TextureRegion(tex2.get());
	GetGameObject()->GetStage()->GetSpriteBatch()->sortMode = SpriteBatch::SortMode::TextureBased;
}

void OptBatch::Update()
{
}

void OptBatch::Draw()
{
	SpriteBatch* batch = GetGameObject()->GetStage()->GetSpriteBatch();
	for (size_t i = 0; i < 100000; i++)
	{
		batch->Draw(&reg1, 0.0f, 0.0f, 20.0f, 20.0f, 20.0f, 0.0, RhyC::white);
		batch->Draw(&reg2, 0.0f, 20.0f, 20.0f, 20.0f, 20.0f, 0.0, RhyC::white);
	}
}
