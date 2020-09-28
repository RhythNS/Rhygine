#include "RhyWin.h"
#include <d3dcompiler.h>

#include "TestScene.h"
#include "RhyException.h"
#include "Window.h"
#include "TestPyramid.h"
#include "TestModel.h"

void TestScene::Init()
{
	gameobjects.push_back(std::make_unique<TestPyramid>());
	gameobjects.push_back(std::make_unique<TestModel>());
	

	for (auto& gameobject : gameobjects)
		gameobject->Init();
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
