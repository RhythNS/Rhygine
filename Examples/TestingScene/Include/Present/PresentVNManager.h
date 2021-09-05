#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>
#include <ECS/Bindable/Texture.h>
#include <UI/Elements/UIContainer.h>
#include <UI/Elements/UIImage.h>

#include <Present/VNTextbox.h>
#include <Present/VNCharacter.h>

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

