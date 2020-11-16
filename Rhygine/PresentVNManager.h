#pragma once
#include "Component.h"
#include "Updatable.h"
#include "Texture.h"
#include "UIContainer.h"
#include "UIImage.h"
#include "VNTextbox.h"
#include "VNCharacter.h"

#include <memory>

class PresentVNManager : public Component, public Updatable
{
public:
	void Init();

	void OnFinish();
private:
	UIContainer* rootContainer;
	//UIImage* rootContainer; // <- remove
	//std::unique_ptr<Texture> rootTex;
	
	UIImage* textBoxBackground;
	VNTextbox* textBox;
	VNCharacter* character;
	std::unique_ptr<Texture> textBoxBackgroundTexture;
	std::unique_ptr<Texture> testTex;
};

