#include "OptScene.h"

#include "OptBatch.h"
#include "OptModel.h"
#include "OptNep.h"
#include "OptBasic.h"

#include "Stage.h"
#include "Gameobject.h"
#include "InfoDisplayer.h"
#include "PresentResources.h"

void OptScene::InnerInit()
{
	stage->CreateGameObject()->AddComponent<PresentResources>();
	stage->CreateGameObject()->AddComponent<InfoDisplayer>();
	stage->CreateGameObject()->AddComponent<OptBasic>();
	stage->CreateGameObject()->AddComponent<OptNep>();
	stage->CreateGameObject()->AddComponent<OptBatch>();
	stage->CreateGameObject()->AddComponent<OptModel>();
}
