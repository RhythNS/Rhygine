#include "TestScene.h"
#include "RhyException.h"
#include "Window.h"

#include <Windows.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <d3d11.h>

void TestScene::Init()
{
	pyramid.Init();
}

void TestScene::Update()
{
	pyramid.Update();
}

void TestScene::Draw()
{
	pyramid.Draw();
}
