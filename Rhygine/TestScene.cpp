#include "TestScene.h"
#include "RhyMath.h"
#include "TestPyramid.h"
#include "TestModel.h"
#include "TestTexture.h"
#include "BlendStateBindable.h"
#include "RotateAround.h"
#include "SpriteBatchTester.h"

void TestScene::InnerInit()
{
	clearColor[0] = 0.2f;
	clearColor[1] = 0.2f;
	clearColor[2] = 0.2f;
	clearColor[3] = 1.0f;

	TestPyramid tp;
	TestModel tm;
	TestTexture tt;
	GameObjectFactory::Add(stage.get(), &tm);
	GameObjectFactory::Add(stage.get(), &tt);
	for (int i = 0; i < 10; i++)
	{
	 	GameObject* pyramid = GameObjectFactory::Add(stage.get(), &tp);
		pyramid->GetComponent<Transform>()->localPosition = RhyM::Vec3(0.0f, 0.0f, i * 2.0f);
		pyramid->GetComponent<Drawer>()->AddBindable(std::make_unique<BlendStateBindable>());
	}

	GameObject* lowerPyramid =GameObjectFactory::Add(stage.get(), &tp);
	Transform* lowerTransform = lowerPyramid->GetComponent<Transform>();
	lowerTransform->localPosition = RhyM::Vec3(0.0f, 2.0f, 0.0f);
	lowerTransform->localScale = RhyM::Vec3(0.5f, 0.5f, 0.5f);
	lowerPyramid->GetComponent<Drawer>()->AddBindable(std::make_unique<BlendStateBindable>());

	GameObject* childPyramid =GameObjectFactory::Add(stage.get(), &tp);
	childPyramid->GetComponent<Transform>()->SetParent(lowerTransform);
	childPyramid->GetComponent<Transform>()->localPosition = RhyM::Vec3(2.0f, 0.0f, 0.0f);
	childPyramid->GetComponent<Transform>()->localScale = RhyM::Vec3(2.0f, 2.0f, 2.0f);
	childPyramid->GetComponent<RotateAround>()->rotationSpeed = RhyM::Vec3(0.0, 1.0f, 0.0f);
	childPyramid->GetComponent<Drawer>()->AddBindable(std::make_unique<BlendStateBindable>());

	stage->GetBack()->AddComponent<SpriteBatchTester>();
}
