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
	if (trans->GetWorldPosition().m_floats[1] < y)
	{
		GetGameObject()->GetStage()->RemoveGameObject(GetGameObject());
	}
}
