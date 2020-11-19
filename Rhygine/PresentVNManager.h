#pragma once
#include "Component.h"
#include "Updatable.h"
#include "Texture.h"
#include "UIContainer.h"
#include "UIImage.h"
#include "VNTextbox.h"
#include "VNCharacter.h"

#include <memory>

/// <summary>
/// Creates the visual novel style text display.
/// </summary>
class PresentVNManager : public Component, public Updatable
{
public:
	void Init();

	void OnFinish();
private:
	UIContainer* rootContainer = nullptr;
	UIImage* textBoxBackground = nullptr;
	VNTextbox* textBox = nullptr;
	VNCharacter* character = nullptr;
};

