#include <Scenes/OptScene.h>
#include <Opt/OptBatch.h>
#include <Opt/OptModel.h>
#include <Opt/OptNep.h>
#include <Opt/OptBasic.h>

#include <ECS/Stage.h>
#include <ECS/GameObject.h>
#include <Components/InfoDisplayer.h>
#include <Present/PresentResources.h>

void OptScene::InnerInit()
{
	stage->CreateGameObject()->AddComponent<PresentResources>();
	stage->CreateGameObject()->AddComponent<InfoDisplayer>();
	stage->CreateGameObject()->AddComponent<OptBasic>();
	stage->CreateGameObject()->AddComponent<OptNep>();
	stage->CreateGameObject()->AddComponent<OptBatch>();
	stage->CreateGameObject()->AddComponent<OptModel>();
}
