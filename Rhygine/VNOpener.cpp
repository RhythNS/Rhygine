#include "VNOpener.h"
#include "Gameobject.h"
#include "PresentVNManager.h"
#include "PresentResources.h"

void VNOpener::Init()
{
	reg = TextureRegion(PresentResources::instance->exclamationMark.get());
	SetTextures(&reg);
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
	r = rIncrementing ? 255 * perc : 255 * (1 - perc);

	color = RhyC::FromARGB(255, r, 255, 255);

	if (deleteReq)
		GetGameObject()->RemoveComponent(this);
}
