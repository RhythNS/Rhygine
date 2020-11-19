#include "UIShowcaser.h"
#include "UIVertPercentageSizer.h"
#include "UIVertGroupSizer.h"
#include "TextureGenerator.h"
#include "Gameobject.h"
#include "Stage.h"
#include "UIRootElement.h"
#include "UIFont.h"
#include "PresentResources.h"
#include "UIFitToParentSizer.h"
#include "UIFitToParentRelativeSizer.h"
#include "CreateButton.h"
#include "DeleteButton.h"
#include "UIContainer.h"

#include <vector>

void UIShowcaser::Init()
{
	instance = this;

	TextureGenerator texGen = TextureGenerator(4, 4, RhyC::black);
	black = std::make_unique<Texture>(&texGen, 0);

	texGen.brush = RhyC::white;
	texGen.Fill(0, 0);
	white = std::make_unique<Texture>(&texGen, 0);
	
	texGen.brush = RhyC::orange;
	texGen.Fill(0, 0);
	orange = std::make_unique<Texture>(&texGen, 0);

	texGen.brush = RhyC::blue;
	texGen.Fill(0, 0);
	blue = std::make_unique<Texture>(&texGen, 0);

	texGen.brush = RhyC::silver;
	texGen.Fill(0, 0);
	silver = std::make_unique<Texture>(&texGen, 0);


	base = GetGameObject()->AddComponent<UIImage>();
	base->SetParent(GetGameObject()->GetStage()->GetUIRoot());
	base->SetSizer(std::make_unique<UIVertPercentageSizer>(std::vector<float>({0.2f, 0.8f, 0.2f})));
	base->SetSize(RhyM::Vec2(200.0f, 400.0f));
	base->SetPos(RhyM::Vec2(100.0f, 100.0f));
	base->image = TextureRegion(black.get());

	
	low = GetGameObject()->AddComponent<UIImage>();
	low->SetParent(base);
	low->image = TextureRegion(white.get());
	
	mid = GetGameObject()->AddComponent<UIImage>();
	mid->SetParent(base);
	mid->SetSizer(std::make_unique<UIVertGroupSizer>(10.0f, 5.0f));
	mid->image = TextureRegion(orange.get());
	
	up = GetGameObject()->AddComponent<UIImage>();
	up->SetParent(base);
	up->image = TextureRegion(white.get());


	TextureRegion clickedTex = TextureRegion(white.get());
	TextureRegion hoverTex = TextureRegion(blue.get());
	TextureRegion normalTex = TextureRegion(silver.get());

	create = GetGameObject()->AddComponent<CreateButton>();
	create->SetParent(up);
	create->SetTextures(&normalTex, &hoverTex, &clickedTex);
	create->SetSizer(std::make_unique<UIFitToParentRelativeSizer>(0.1f, 0.9f, 0.1f, 0.9f));

	UIFont* fontOnButton = GetGameObject()->AddComponent<UIFont>();
	fontOnButton->Set(&PresentResources::instance->defFont, false, UIFont::VertAlignment::Middle, UIFont::HoriAlignment::Middle, 0.3f);
	fontOnButton->text = "Create";
	fontOnButton->SetParent(create);
	fontOnButton->SetSizer(std::make_unique<UIFitToParentSizer>());


	remove = GetGameObject()->AddComponent<DeleteButton>();
	remove->SetParent(low);
	remove->SetTextures(&normalTex, &hoverTex, &clickedTex);
	remove->SetSizer(std::make_unique<UIFitToParentRelativeSizer>(0.1f, 0.9f, 0.1f, 0.9f));

	fontOnButton = GetGameObject()->AddComponent<UIFont>();
	fontOnButton->Set(&PresentResources::instance->defFont, false, UIFont::VertAlignment::Middle, UIFont::HoriAlignment::Middle, 0.3f);
	fontOnButton->text = "Delete";
	fontOnButton->SetParent(remove);
	fontOnButton->SetSizer(std::make_unique<UIFitToParentSizer>());
}

void UIShowcaser::Update()
{
}

void UIShowcaser::MakeButton()
{
	UIButton* onVert = GetGameObject()->AddComponent<UIButton>();
	onVert->SetParent(mid);
	TextureRegion clickedTex = TextureRegion(white.get());
	TextureRegion hoverTex = TextureRegion(blue.get());
	TextureRegion normalTex = TextureRegion(silver.get());
	onVert->SetTextures(&normalTex, &hoverTex, &clickedTex);

	UIFont* fontOnButton = GetGameObject()->AddComponent<UIFont>();
	fontOnButton->Set(&PresentResources::instance->defFont, false, UIFont::VertAlignment::Middle, UIFont::HoriAlignment::Middle, 0.3f);
	fontOnButton->text = "Press me!";
	fontOnButton->SetParent(onVert);
	fontOnButton->SetSizer(std::make_unique<UIFitToParentSizer>());
}

void UIShowcaser::RemoveButton()
{
	if (mid->GetChildrenCount() != 0)
	{
		GetGameObject()->RemoveComponent(mid->GetChildAt(0));
	}
}

UIShowcaser* UIShowcaser::instance;
