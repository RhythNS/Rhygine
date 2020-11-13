#include "Scene.h"
#include "Stage.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraMover.h"

void Scene::Init()
{
	stage = std::make_unique<Stage>();
	stage->Get3DCamera()->GetGameObject()->AddComponent<CameraMover>();
}

void Scene::Update()
{
	InnerPreUpdate();
	stage->Update();
	InnerAfterUpdate();
}

void Scene::Draw()
{
	InnerPreDraw();
	stage->Draw();
	InnerAfterDraw();
}

float* Scene::GetClearColor()
{
	return clearColor;
}
