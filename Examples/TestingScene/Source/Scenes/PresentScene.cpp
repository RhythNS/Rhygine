#include <Scenes/PresentScene.h>
#include <ECS/GameObject.h>

#include <Factories/TestBasicLit.h>
#include <Factories/TestNeptuneLoader.h>
#include <Factories/TestModelLoader.h>
#include <Factories/TestCrystal.h>
#include <Factories/TestLitPlate.h>
#include <Factories/TestLight.h>
#include <Factories/TestFloor.h>

#include <Components/InfoDisplayer.h>
#include <Components/RotateAround.h>
#include <Components/TestLitPlateComponent.h>
//#include <Components/BulletShowcaser.h>
#include <Components/ReactShowcaser.h>
#include <CC/TestLightComponent.h>
#include <UI/Elements/UIRootElement.h>

#include <React/ReactPhysicsModule.h>
#include <Core/Window.h>

#include <ECS/Bindable/Drawer.h>
#include <ECS/Bindable/Texture.h>
#include <CC/Shaders/TexLitShader.h>

#include <Present/PresentResources.h>
#include <Present/PresentVNManager.h>
#include <Present/VNOpener.h>

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


	TestBasicLit tbl = TestBasicLit("Assets\\PresentScene\\carpet.jpg", tlc);
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

	//stage->CreateGameObject()->AddComponent<BulletShowcaser>();
	stage->CreateGameObject()->AddComponent<ReactShowcaser>();
	//Window::GetInstance()->GetModule<RhyReact::ReactPhysicsModule>()->EnableDebug(this);
}
