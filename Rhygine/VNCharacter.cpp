#include "VNCharacter.h"
#include "PresentResources.h"

void VNCharacter::Init()
{
	happy = PresentResources::instance->happy.get();
	mad = PresentResources::instance->mad.get();
	sad = PresentResources::instance->sad.get();
	surprised = PresentResources::instance->surprised.get();
	embarrassed = PresentResources::instance->embarrassed.get();
}

void VNCharacter::InnerDraw(SpriteBatch* batch)
{
	Texture* tex = nullptr;
	switch (currentEmotion)
	{
	case VNCharacter::Emotion::Happy:
		tex = happy;
		break;
	case VNCharacter::Emotion::Mad:
		tex = mad;
		break;
	case VNCharacter::Emotion::Sad:
		tex = sad;
		break;
	case VNCharacter::Emotion::Suprised:
		tex = surprised;
		break;
	case VNCharacter::Emotion::Embarrassed:
		tex = embarrassed;
		break;
	default:
		assert(false);
		break;
	}

	DrawTexture(batch, tex);
}

RhyM::Vec2 VNCharacter::GetPrefSize()
{
	return RhyM::Vec2(static_cast<float>(happy->GetWidth()), static_cast<float>(happy->GetHeight()));
}
