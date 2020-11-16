#pragma once
#include "UIElement.h"
#include "Texture.h"

class VNCharacter : public UIElement
{
public:
	enum class Emotion {
		Happy, Mad, Sad, Suprised, Embarrassed
	};

	void Init();
	void InnerDraw(SpriteBatch* batch);
	RhyM::Vec2 GetPrefSize();


	Emotion currentEmotion = Emotion::Happy;
private:
	std::unique_ptr<Texture> happy, mad, sad, surprised, embarrassed;
};
