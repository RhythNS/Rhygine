#include "PresentScene.h"
#include "Gameobject.h"
#include "PresentResources.h"
#include "PresentVNManager.h"

#include "TestLitPlate.h"
#include "TestLight.h"
#include "TestLightComponent.h"
#include "TestLitPlateComponent.h"

#include "RotateAround.h"

void PresentScene::InnerInit()
{
	stage->GetFront()->AddComponent<PresentResources>();

	// TODO remove
	//stage->CreateGameObject()->AddComponent<PresentVNManager>();

	TestLight tl;
	TestLitPlate tlp;

	TestLightComponent* tlc = GameObjectFactory::Add(stage.get(), &tl)->GetComponent<TestLightComponent>();

	GameObject* lit = GameObjectFactory::Add(stage.get(), &tlp);
	lit->GetComponent<TestLitPlateComponent>()->SetLight(0, tlc);
	lit->GetComponent<Transform>()->localPosition.setValue(-2.0f, 0.0f, 0.0f);
	lit->GetComponent<Transform>()->localScale.setValue(1.0f, 1.0f, 1.0f);
	lit->GetComponent<RotateAround>()->Enable();

}
