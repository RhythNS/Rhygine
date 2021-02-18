#include "PresentVNManager.h"
#include "Gameobject.h"
#include "Stage.h"
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
	rootContainer->SetOwnSizer(std::make_unique<UIKeepWidth>());

	character = GetGameObject()->AddComponent<VNCharacter>();
	character->SetParent(rootContainer);
	character->SetSize(character->GetPrefSize());
	character->SetOwnSizer(std::make_unique<UIKeepWidth>());


	Texture* textBoxBackgroundTexture = PresentResources::instance->textBoxBackgroundTexture.get();
	textBoxBackground = GetGameObject()->AddComponent<UIImage>();
	textBoxBackground->SetParent(rootContainer);
	textBoxBackground->image = TextureRegion(textBoxBackgroundTexture);
	textBoxBackground->SetSize(RhyM::Vec2(static_cast<float>(textBoxBackgroundTexture->GetWidth()), static_cast<float>(textBoxBackgroundTexture->GetHeight())));
	textBoxBackground->SetOwnSizer(std::make_unique<UIKeepWidth>(VertAlignment::Down, HoriAlignment::Middle, 120.0f, 50.0f));

	std::vector<VNTextbox::Node> scene =
	{
		VNTextbox::Node("Hey, nice to meet you.\nI'm Hoshiko, and I am going to tell you about the scene I am in.", VNCharacter::Emotion::Happy),
		VNTextbox::Node("Usually, this would be presented by Unitychan.\nHowever, she said that she only wanted to appear in Unity games.", VNCharacter::Emotion::Sad),
		VNTextbox::Node("I am an example of a UI element.\nYou can try to resize the game window, in order to test my\nresponsiveness.", VNCharacter::Emotion::Suprised),
		VNTextbox::Node("The three rotating floating cubes show some simple shader\neffects. They show examples of a Rough, Matte and Glossy\nlighting effect.", VNCharacter::Emotion::Happy),
		VNTextbox::Node("Everything about the light can be changed in the little\nlight window. You can drag on the position or colour field to\nchange them. You can also change the colour by clicking on the\npreview colour.", VNCharacter::Emotion::Happy),
		VNTextbox::Node("To the left of that, there is a more complex 3D model, which\nuses a toon-shade effect.", VNCharacter::Emotion::Happy),
		VNTextbox::Node("To the right are some unlit boxes to show the capability of the bullet3 physics engine.", VNCharacter::Emotion::Happy),
		VNTextbox::Node("The wooden floor uses a normal texture to make it feel more\n3D like.", VNCharacter::Emotion::Suprised),
		VNTextbox::Node("I hope that you find your way around the source code.\nHopefully, my creator did not make it to hard for you.", VNCharacter::Emotion::Embarrassed),
		VNTextbox::Node("He wanted me to remind you of the class diagram that is in the\nroot folder of the project, to make your life a bit easier.", VNCharacter::Emotion::Happy),
		VNTextbox::Node("Thank you for checking out my project!", VNCharacter::Emotion::Happy)
	};

	textBox = GetGameObject()->AddComponent<VNTextbox>();
	textBox->SetParent(textBoxBackground);
	textBox->Set(&PresentResources::instance->defFont, true, UIFont::VertAlignment::Up, UIFont::HoriAlignment::Left);
	textBox->SetOwnSizer(std::make_unique<UIFitToParentRelativeSizer>(0.07f, 0.97f, 0.18f, 0.82f));
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
