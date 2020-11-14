#include "UITester.h"

#include "UIElement.h"
#include "UIImage.h"
#include "UIButton.h"
#include "UIContainer.h"
#include "UIFitToParentSizer.h"
#include "UIHoriGroupSizer.h"
#include "UIVertGroupSizer.h"
#include "UILocalSizer.h"
#include "UIVertGroupSizer.h"
#include "UIRootElement.h"

#include "TextureGenerator.h"
#include "Gameobject.h"
#include "Stage.h"

void UITester::Init()
{
	TextureGenerator gen = TextureGenerator(3, 3);

	white = std::make_unique<Texture>(&gen, 0);

	gen.brush = RhyC::blue;
	gen.Fill(0, 0);
	blue = std::make_unique<Texture>(&gen, 0);

	gen.brush = RhyC::black;
	gen.Fill(0, 0);
	black = std::make_unique<Texture>(&gen, 0);

	gen.brush = RhyC::orange;
	gen.Fill(0, 0);
	orange = std::make_unique<Texture>(&gen, 0);

	gen.brush = RhyC::silver;
	gen.Fill(0, 0);
	silver = std::make_unique<Texture>(&gen, 0);

	sprite = std::make_unique<Texture>("TestModels\\Sprite\\Walk (1).png", 0);

	GameObject* game = GetGameObject()->GetStage()->CreateGameObject();
		
	UIImage* back = game->AddComponent<UIImage>();
	back->SetParent(GetGameObject()->GetStage()->GetUIRoot());
	UIVertGroupSizer* backSizer = dynamic_cast<UIVertGroupSizer*>(back->SetSizer(std::make_unique<UIVertGroupSizer>(10.0f, 30.0f)));
	back->image = TextureRegion(0.0f, 0.0f, orange->GetWidth(), orange->GetHeight(), orange.get());
	back->SetPosSizeScale(100.0f, 100.0f, 20.0f, 300.0f, 300.0f, 1.0f, 1.0f);

	UIButton* verts[4];
	for (int i = 0; i < 4; i++)
	{
		UIButton* onVert = game->AddComponent<UIButton>();
		onVert->SetParent(back);
		TextureRegion clickedTex = TextureRegion(0.0f, 0.0f, white->GetWidth(), white->GetHeight(), white.get());
		TextureRegion hoverTex = TextureRegion(0.0f, 0.0f, blue->GetWidth(), blue->GetHeight(), blue.get());
		TextureRegion normalTex = TextureRegion(0.0f, 0.0f, silver->GetWidth(), silver->GetHeight(), silver.get());
		onVert->SetTextures(&normalTex, &hoverTex, &clickedTex);
	}
}
