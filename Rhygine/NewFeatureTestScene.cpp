#include "NewFeatureTestScene.h"
#include "Stage.h"
#include "Gameobject.h"
#include "InfoDisplayer.h"
#include "PresentResources.h"
#include "TestParallelMaker.h"
#include "TestParallelAsUpdate.h"
#include "Window.h"
#include "TaskManager.h"
#include "CoroutineTestComponent.h"
#include "ChangeSceneTest.h"

void NewFeatureTestScene::InnerInit()
{
	stage->CreateGameObject()->AddComponent<PresentResources>();
	stage->CreateGameObject()->AddComponent<InfoDisplayer>();
	// TestParallelAsUpdate::AddToStage(stage.get());
	// TestParallelMaker::AddToStage(stage.get());
	// Window::GetInstance()->GetTaskManager()->AddOneShot(&print);
	stage->CreateGameObject()->AddComponent<CoroutineTestComponent>();
	stage->CreateGameObject()->AddComponent<ChangeSceneTest>();
}
