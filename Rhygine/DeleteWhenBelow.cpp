#include "DeleteWhenBelow.h"
#include "Gameobject.h"
#include "Stage.h"
#include "BulletShowcaser.h"

void DeleteWhenBelow::Init()
{
	trans = GetGameObject()->GetComponent<Transform>();
}

void DeleteWhenBelow::Update()
{
	if (trans->GetWorldPosition().y < y)
	{
		GetGameObject()->GetStage()->RemoveGameObject(GetGameObject());
	}
}
