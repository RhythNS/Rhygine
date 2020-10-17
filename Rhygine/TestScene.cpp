#include "TestScene.h"
#include "RhyMath.h"
#include "TestPyramid.h"
#include "TestModel.h"
#include "TestTexture.h"

void TestScene::InnerInit()
{
	clearColor[0] = 0.2f;
	clearColor[1] = 0.2f;
	clearColor[2] = 0.2f;
	clearColor[3] = 1.0f;

	TestPyramid tp;
	TestModel tm;
	TestTexture tt;
	for (int i = 0; i < 10; i++)
	{
		GameObjectFactory::Add(stage.get(), &tp)->GetComponent<Transform>()->position = RhyM::Vec3(0.0f, 0.0f, i * 2.0f);
	}

	GameObjectFactory::Add(stage.get(), &tm);
	GameObjectFactory::Add(stage.get(), &tt);
}
