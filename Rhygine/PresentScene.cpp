#include "PresentScene.h"
#include "Gameobject.h"

#include "PresentResources.h"
#include "PresentVNManager.h"
#include "VNOpener.h"
#include "UIRootElement.h"
#include "InfoDisplayer.h"

#include "RotateAround.h"
#include "Drawer.h"
#include "Texture.h"
#include "TexLitShader.h"
#include "TestBasicLit.h"
#include "TestNeptuneLoader.h"
#include "TestModelLoader.h"
#include "TestCrystal.h"
#include "TestLitPlate.h"
#include "TestLight.h"
#include "TestLightComponent.h"
#include "TestLitPlateComponent.h"
#include "TestFloor.h"
#include "BulletShowcaser.h"

void PresentScene::InnerInit()
{
	clearColor[0] = 0.5f;
	clearColor[1] = 0.5f;
	clearColor[2] = 0.5f;
	clearColor[3] = 1.0f;

	stage->GetFront()->AddComponent<PresentResources>();
	stage->CreateGameObject()->AddComponent<InfoDisplayer>();

	VNOpener* opener = stage->CreateGameObject()->AddComponent<VNOpener>();
	opener->SetParent(stage->GetUIRoot());
	opener->SetSize(100.0f, 100.0f);
	opener->SetPos(0.0f, 0.0f);

	TestLight tl;
	TestLightComponent* tlc = GameObjectFactory::Add(stage.get(), &tl)->GetComponent<TestLightComponent>();
	

	TestLitPlate tlp = TestLitPlate(tlc);
	GameObject* litMovingSprite = GameObjectFactory::Add(stage.get(), &tlp);
	litMovingSprite->GetComponent<Transform>()->localPosition.Set(-2.0f, 0.0f, 0.0f);
	litMovingSprite->GetComponent<Transform>()->localScale.Set(1.0f, 1.0f, 1.0f);
	litMovingSprite->GetComponent<RotateAround>()->Enable();


	TestFloor tf = TestFloor(15.0f, 15.0f, tlc);
	GameObject* floor = GameObjectFactory::Add(stage.get(), &tf);
	floor->GetComponent<Transform>()->localPosition.Set(0.0f, -2.0f, 0.0f);


	TestBasicLit tbl = TestBasicLit("PresentScene\\carpet.jpg", tlc);
	GameObject* litCarpet = GameObjectFactory::Add(stage.get(), &tbl);
	litCarpet->GetComponent<Transform>()->localPosition.Set(2.0f, 0.0f, 0.0f);
	litCarpet->GetComponent<Transform>()->localScale.Set(1.0f, 1.0f, 1.0f);
	litCarpet->GetComponent<RotateAround>()->rotationSpeed = RhyM::Vec3(0.2f, 0.5f, 1.0f);
	litCarpet->GetComponent<RotateAround>()->Enable();

	TestModelLoader testLoader = TestModelLoader(tlc);
	GameObjectFactory::Add(stage.get(), &testLoader);

	TestNeptuneLoader nepLoader = TestNeptuneLoader(tlc);
	GameObjectFactory::Add(stage.get(), &nepLoader);

	TestCrystal crystal(tlc);
	GameObjectFactory::Add(stage.get(), &crystal);

	stage->CreateGameObject()->AddComponent<BulletShowcaser>();
}
