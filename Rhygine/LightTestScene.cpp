#include "LightTestScene.h"
#include "RhyMath.h"

#include "TestLight.h"
#include "TestPyramid.h"
#include "TestLitPlate.h"
#include "TestLightComponent.h"
#include "TestLitPlateComponent.h"

#include <memory>
#include <array>

void LightTestScene::Init()
{
	TestLight tl;
	TestLitPlate tlp;
	TestLightComponent* tlc = GameObjectFactory::Add(&gameobjects, &tl)->GetComponent<TestLightComponent>();
	GameObjectFactory::Add(&gameobjects, &tlp)->GetComponent<TestLitPlateComponent>()->SetLight(0, tlc);

	for (auto& gameobject : gameobjects)
		gameobject->Init();
}
