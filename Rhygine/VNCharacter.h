#pragma once
#include "UIElement.h"
#include "Texture.h"

/// <summary>
/// Character which chooses an image to display dependent to the set emotion.
/// </summary>
class VNCharacter : public UIElement
{
public:
	enum class Emotion {
		Happy, Mad, Sad, Suprised, Embarrassed
	};

	void Init();
	void InnerDraw(SpriteBatch* batch);
	/// <summary>
	/// Gets the preferred size of the character.
	/// </summary>
	/// <returns></returns>
	RhyM::Vec2 GetPrefSize();

	Emotion currentEmotion = Emotion::Happy;

private:
	Texture *happy, *mad, *sad, *surprised, *embarrassed;
};
