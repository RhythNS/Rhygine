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
#include "UIFont.h"
#include "MonoFont.h"

#include "TextureGenerator.h"
#include "Gameobject.h"
#include "Stage.h"
#include "Rhyimgui.h"

void UITester::Init()
{
	std::string str;
	for (char i = 32; i < 127; i++)
	{
		str += i;
	}
	OutputDebugString(str.c_str());


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

	font = std::make_unique<MonoFont>("PresentScene\\SourceCodePro_Regular_(72)_[0,0,0,0]_45x76.png", 45, 76);

	GameObject* game = GetGameObject()->GetStage()->CreateGameObject();
	UIRootElement* root = GetGameObject()->GetStage()->GetUIRoot();

	
	UIFont* test1 = game->AddComponent<UIFont>();
	test1->Set(font.get(), false, UIFont::VertAlignment::Middle, UIFont::HoriAlignment::Middle, 0.3f);
	test1->text = "This is a multiline text!\nYooo I hope this works!";
	test1->SetParent(root);
	test1->SetSizer(std::make_unique<UIFitToParentSizer>());
	test1->color = RhyC::black;
	
	UIFont* test2 = game->AddComponent<UIFont>();
	test2->Set(font.get(), false, UIFont::VertAlignment::Up, UIFont::HoriAlignment::Left, 0.3f);
	test2->text = "This is a multiline text!\nYooo I hope this works!";
	test2->SetParent(root);
	test2->SetSizer(std::make_unique<UIFitToParentSizer>());
	
	UIFont* test3 = game->AddComponent<UIFont>();
	test3->Set(font.get(), false, UIFont::VertAlignment::Down, UIFont::HoriAlignment::Right, 0.3f);
	test3->text = "This is a multiline text!\nYooo I hope this works!";
	test3->SetParent(root);
	test3->SetSizer(std::make_unique<UIFitToParentSizer>());
	

	UIImage* back = game->AddComponent<UIImage>();
	back->SetParent(root);
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

		UIFont* fontOnButton = game->AddComponent<UIFont>();
		fontOnButton->Set(font.get(), false, UIFont::VertAlignment::Middle, UIFont::HoriAlignment::Middle, 0.3f);
		fontOnButton->text = "Press me!";
		fontOnButton->SetParent(onVert);
		fontOnButton->SetSizer(std::make_unique<UIFitToParentSizer>());
	}
}

void UITester::Update()
{
	RhyM::Vec3 pos = GetGameObject()->GetStage()->GetUICamera()->position;
	ImGui::Begin("UI Test");
	ImGui::Text((std::to_string(pos.m_floats[0]) + " " + std::to_string(pos.m_floats[1])).c_str());
	ImGui::End();
}
