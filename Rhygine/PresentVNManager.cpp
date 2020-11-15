#include "PresentVNManager.h"
#include "Gameobject.h"
#include "Stage.h"
#include "UIRootElement.h"
#include "UIKeepWidthSizer.h"
#include "TextureGenerator.h"


void PresentVNManager::Init()
{
	textBoxBackgroundTexture = std::make_unique<Texture>("PresentScene\\textBox.png", 0);

	TextureGenerator texGen = TextureGenerator(2, 2, RhyC::orange);
	rootTex = std::make_unique<Texture>(&texGen, 0);

	//rootContainer = GetGameObject()->AddComponent<UIContainer>();
	rootContainer = GetGameObject()->AddComponent<UIImage>();
	rootContainer->image = TextureRegion(rootTex.get());
	rootContainer->SetParent(GetGameObject()->GetStage()->GetUIRoot());
	rootContainer->SetSize(RhyM::Vec2(1300, 800));
	rootContainer->SetSizer(std::make_unique<UIKeepWidth>());

		//1920x1080
	character = GetGameObject()->AddComponent<VNCharacter>();
	character->SetParent(rootContainer);
	character->SetSize(character->GetPrefSize());
	character->SetSizer(std::make_unique<UIKeepWidth>());

	/*
	textBoxBackground = GetGameObject()->AddComponent<UIImage>();
	textBoxBackground->image = TextureRegion(textBoxBackgroundTexture.get());
	*/

	/*
	
	font = std::make_unique<MonoFont>("PresentScene\\SourceCodePro_Regular_(72)_[0,0,0,0]_45x76.png", 45, 76);
	*/
}
