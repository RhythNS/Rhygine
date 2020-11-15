#include "VNCharacter.h"

void VNCharacter::Init()
{
	happy = std::make_unique<Texture>("PresentScene\\happy.png", 0);
	//mad = std::make_unique<Texture>("PresentScene\\mad.png", 0);
	//sad = std::make_unique<Texture>("PresentScene\\sad.png", 0);
	//suprised = std::make_unique<Texture>("PresentScene\\suprised.png", 0);
	//embarrassed = std::make_unique<Texture>("PresentScene\\embarrassed.png", 0);
}

void VNCharacter::InnerDraw(SpriteBatch* batch)
{
	Texture* tex = nullptr;
	switch (currentEmotion)
	{
	case VNCharacter::Emotion::Happy:
		tex = happy.get();
		break;
	case VNCharacter::Emotion::Mad:
		tex = mad.get();
		break;
	case VNCharacter::Emotion::Sad:
		tex = sad.get();
		break;
	case VNCharacter::Emotion::Suprised:
		tex = suprised.get();
		break;
	case VNCharacter::Emotion::Embarrassed:
		tex = embarrassed.get();
		break;
	default:
		assert(false);
		break;
	}

	DrawTexture(batch, tex);
}

RhyM::Vec2 VNCharacter::GetPrefSize()
{
	return RhyM::Vec2(happy->GetWidth(), happy->GetHeight());
}
