#include "RhyWin.h"
#include "RhyMath.h"
#include "TestScene.h"
#include "RhyException.h"
#include "Window.h"
#include "TestPyramid.h"
#include "TestModel.h"
#include "TestTexture.h"

#include <d3dcompiler.h>

void TestScene::Init()
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
		GameObjectFactory::Add(&gameobjects, &tp)->GetComponent<Transform>()->position = RhyM::Vec3(0.0f, 0.0f, i * 2.0f);
	}

	GameObjectFactory::Add(&gameobjects, &tm);
	GameObjectFactory::Add(&gameobjects, &tt);
}

void TestScene::Update()
{
	for (auto& gameobject : gameobjects)
		gameobject->Update();
}

void TestScene::Draw()
{
	for (auto& gameobject : gameobjects)
		gameobject->Draw();
}
