#include "LightTestScene.h"
#include "RhyMath.h"

#include "TestLight.h"
#include "TestPyramid.h"
#include "TestLitPlate.h"

#include <memory>
#include <array>

void LightTestScene::Init()
{
	/*
	float dir[3] = { 0.0f, 0.0f, 0.0f };
	gameobjects.push_back(std::make_unique<TestLight>(0, dir));
	std::array<TestLight*, 10> lights = { static_cast<TestLight*> (gameobjects[gameobjects.size() - 1].get()) };

	gameobjects.push_back(std::make_unique<TestLitPlate>(lights));
	//gameobjects.push_back(std::make_unique<TestPyramid>(Vec3(0.0f,0.0f,10.0f)));

	for (auto& gameobject : gameobjects)
		gameobject->Init();
	*/
}
