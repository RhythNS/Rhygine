#include "PresentVNManager.h"
#include "Gameobject.h"
#include "Stage.h"
#include "UISizer.h"
#include "UIFitToParentRelativeSizer.h"
#include "UIRootElement.h"
#include "UIKeepWidthSizer.h"
#include "TextureGenerator.h"
#include "PresentResources.h"
#include "Camera.h"
#include "CameraMover.h"

#include <functional>

void PresentVNManager::Init()
{
	GetGameObject()->GetStage()->Get3DCamera()->GetGameObject()->GetComponent<CameraMover>()->Disable();

	rootContainer = GetGameObject()->AddComponent<UIContainer>();
	rootContainer->SetParent(GetGameObject()->GetStage()->GetUIRoot());
	rootContainer->SetSize(RhyM::Vec2(1300, 800));
	rootContainer->SetSizer(std::make_unique<UIKeepWidth>());

	character = GetGameObject()->AddComponent<VNCharacter>();
	character->SetParent(rootContainer);
	character->SetSize(character->GetPrefSize());
	character->SetSizer(std::make_unique<UIKeepWidth>());


	Texture* textBoxBackgroundTexture = PresentResources::instance->textBoxBackgroundTexture.get();
	textBoxBackground = GetGameObject()->AddComponent<UIImage>();
	textBoxBackground->SetParent(rootContainer);
	textBoxBackground->image = TextureRegion(textBoxBackgroundTexture);
	textBoxBackground->SetSize(RhyM::Vec2(static_cast<float>(textBoxBackgroundTexture->GetWidth()), static_cast<float>(textBoxBackgroundTexture->GetHeight())));
	textBoxBackground->SetSizer(std::make_unique<UIKeepWidth>(UISizer::VertAlignment::Down, UISizer::HoriAlignment::Middle, 120.0f, 50.0f));

	std::vector<VNTextbox::Node> scene =
	{
		VNTextbox::Node("This is a test. Yay yay!\nLet us see what will happen!", VNCharacter::Emotion::Happy),
		VNTextbox::Node("Yay?", VNCharacter::Emotion::Sad),
		VNTextbox::Node("It acctually worked?", VNCharacter::Emotion::Suprised)
	};

	textBox = GetGameObject()->AddComponent<VNTextbox>();
	textBox->SetParent(textBoxBackground);
	textBox->Set(&PresentResources::instance->defFont, true, UIFont::VertAlignment::Up, UIFont::HoriAlignment::Left);
	textBox->SetSizer(std::make_unique<UIFitToParentRelativeSizer>(0.07f, 0.97f, 0.18f, 0.82f));
	textBox->DisplayScene(scene, character, std::bind(&PresentVNManager::OnFinish, this), textBoxBackgroundTexture->GetWidth() * 2.0f);
}

void PresentVNManager::OnFinish()
{
	GetGameObject()->GetStage()->Get3DCamera()->GetGameObject()->GetComponent<CameraMover>()->Enable();

	GetGameObject()->RemoveComponent(textBox);
	GetGameObject()->RemoveComponent(textBoxBackground);
	GetGameObject()->RemoveComponent(character);
	GetGameObject()->RemoveComponent(rootContainer);

	GetGameObject()->GetStage()->RemoveGameObject(GetGameObject());
}
