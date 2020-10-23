#include "LightTestScene.h"
#include "RhyMath.h"

#include "TestLight.h"
#include "TestPyramid.h"
#include "TestLitPlate.h"
#include "TestModel.h"
#include "TestLightComponent.h"
#include "TestLitPlateComponent.h"
#include "Transform.h"
#include "RotateAround.h"
#include "TextureChanger.h"
#include "Drawer.h"
#include "Texture.h"

#include <memory>
#include <array>

void LightTestScene::InnerInit()
{
	TestLight tl;
	TestLitPlate tlp;
	TestPyramid tp;
	TestModel tm;

	TestLightComponent* tlc = GameObjectFactory::Add(stage.get(), &tl)->GetComponent<TestLightComponent>();

	/*
	for (int i = -1; i < 2; i++)
	{
		GameObject* o = GameObjectFactory::Add(stage.get(), &tlp);
		o->GetComponent<TestLitPlateComponent>()->SetLight(0, tlc);
		o->GetComponent<Transform>()->position.Set(i * 2.0f, 0.0f, 0.0f);
		o->GetComponent<Transform>()->scale.Set(1.0f, 1.0f, 1.0f);
	}
	*/

	GameObject* lit = GameObjectFactory::Add(stage.get(), &tlp);
	lit->GetComponent<TestLitPlateComponent>()->SetLight(0, tlc);
	lit->GetComponent<Transform>()->position.Set(-2.0f, 0.0f, 0.0f);
	lit->GetComponent<Transform>()->scale.Set(1.0f, 1.0f, 1.0f);
	lit->GetComponent<RotateAround>()->Enable();

	GameObject* lit2 = GameObjectFactory::Add(stage.get(), &tlp);
	lit2->GetComponent<TestLitPlateComponent>()->SetLight(0, tlc);
	Drawer* drawer = lit2->GetComponent<Drawer>();
	drawer->RemoveBindable(drawer->GetBindable<TextureChanger>());
	drawer->AddBindable(std::make_unique<Texture>("TestModels\\Sprite\\TestImage.png", 0));
	lit2->GetComponent<Transform>()->position.Set(0.0f, 1.0f, 0.0f);
	lit2->GetComponent<Transform>()->scale.Set(1.0f, 1.0f, 1.0f);
	lit2->GetComponent<RotateAround>()->rotationSpeed.Set(0.0f, 0.0f, 1.0f);
	lit2->GetComponent<RotateAround>()->Enable();

	GameObject* pyramid = GameObjectFactory::Add(stage.get(), &tp);
	pyramid->GetComponent<Transform>()->position.Set(0.0f, -1.0f, 0.0f);
	pyramid->GetComponent<RotateAround>()->rotationSpeed.Set(1.0f, 0.0f, 0.0f);
	
	GameObject* model = GameObjectFactory::Add(stage.get(), &tm);
	model->GetComponent<Transform>()->position.Set(2.0f, 0.0f, 0.0f);
	model->AddComponent<RotateAround>()->rotationSpeed.Set(1.0f, 1.0f, 1.0f);

}
