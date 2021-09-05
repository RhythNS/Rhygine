#include <Components/ChangeSceneTest.h>
#include <Scenes/TestScene.h>
#include <Scenes/LightTestScene.h>
#include <Scenes/BulletTestScene.h>
#include <Scenes/MemoryLeakTestScene.h>
#include <Scenes/NewFeatureTestScene.h>
#include <Scenes/OptScene.h>
#include <Scenes/PresentScene.h>
#include <Core/Window.h>
#include <ECS/GameObject.h>

void ChangeSceneTest::Init()
{
	GetGameObject()->survivesSceneChange = true;
}

void ChangeSceneTest::Update()
{
	if (GetKeys()->IsKeyDownThisFrame('1'))
		Window::GetInstance()->ChangeScene(new BulletTestScene());
	
	if (GetKeys()->IsKeyDownThisFrame('2'))
		Window::GetInstance()->ChangeScene(new TestScene());
	
	if (GetKeys()->IsKeyDownThisFrame('3'))
		Window::GetInstance()->ChangeScene(new LightTestScene());
	
	if (GetKeys()->IsKeyDownThisFrame('4'))
		Window::GetInstance()->ChangeScene(new OptScene());
	
	if (GetKeys()->IsKeyDownThisFrame('5'))
		Window::GetInstance()->ChangeScene(new PresentScene());
		
	if (GetKeys()->IsKeyDownThisFrame('6'))
		Window::GetInstance()->ChangeScene(new MemoryLeakTestScene());
	
	if (GetKeys()->IsKeyDownThisFrame('7'))
		Window::GetInstance()->ChangeScene(new NewFeatureTestScene());
}
