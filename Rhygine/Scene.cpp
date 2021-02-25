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

void Scene::OnResize(int newWidth, int newHeight)
{
	stage->OnResize(newWidth, newHeight);
}

void Scene::OnMouseMove(int x, int y)
{
	stage->OnMouseMove(x, y);
}

void Scene::OnMouseDown()
{
	stage->OnMouseDown();
}

void Scene::OnMouseUp()
{
	stage->OnMouseUp();
}

void Scene::OnSceneChange(Scene* prevScene)
{
	Init();
	stage->AddSurvivors(prevScene->stage->GetSurvivors());
	InnerInit();
}

float* Scene::GetClearColor()
{
	return clearColor;
}
