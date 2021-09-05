#include <Present/VNOpener.h>
#include <ECS/GameObject.h>
#include <Present/PresentVNManager.h>
#include <Present/PresentResources.h>
#include <UI/Sizers/UIFitToParentSizer.h>

void VNOpener::Init()
{
	reg = TextureRegion(PresentResources::instance->exclamationMark.get());
	SetTextures(&reg);
	font = GetGameObject()->AddComponent<UIFont>();
	font->SetParent(this);
	font->Set(&PresentResources::instance->defFont, true, UIFont::VertAlignment::Middle, UIFont::HoriAlignment::Middle, 0.3f);
	font->SetOwnSizer(std::make_unique<UIFitToParentSizer>());
	font->text = "Click\nMe!";
	font->color = RhyC::black;
}

void VNOpener::OnClick()
{
	GameObject* go = GetGameObject();
	go->AddComponent<PresentVNManager>();
	deleteReq = true;
}

void VNOpener::Update()
{
	timer += GetDelta();
	if (timer > timeForFullR)
	{
		timer = 0;
		rIncrementing = !rIncrementing;
	}

	float perc = timer / timeForFullR;
	r = rIncrementing ? static_cast<unsigned char>(255 * perc) : static_cast<unsigned char>(255 * (1 - perc));

	color = RhyC::FromRGBA(r, 122, 122, 255);

	if (deleteReq)
	{
		GetGameObject()->RemoveComponent(font);
		GetGameObject()->RemoveComponent(this);
	}
}
