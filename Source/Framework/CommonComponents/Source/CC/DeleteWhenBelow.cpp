#include <CC/DeleteWhenBelow.h>
#include <ECS/GameObject.h>
#include <ECS/Stage.h>

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
