#include "PresentScene.h"
#include "Gameobject.h"
#include "PresentResources.h"
#include "PresentVNManager.h"

void PresentScene::InnerInit()
{
	stage->GetFront()->AddComponent<PresentResources>();

	// TODO remove
	stage->CreateGameObject()->AddComponent<PresentVNManager>();
}
