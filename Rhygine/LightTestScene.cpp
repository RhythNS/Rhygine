#include "LightTestScene.h"
#include "RhyMath.h"

#include "TestLight.h"
#include "TestPyramid.h"
#include "TestLitPlate.h"
#include "TestLightComponent.h"
#include "TestLitPlateComponent.h"

#include <memory>
#include <array>

void LightTestScene::InnerInit()
{
	TestLight tl;
	TestLitPlate tlp;
	TestLightComponent* tlc = GameObjectFactory::Add(stage.get(), &tl)->GetComponent<TestLightComponent>();
	GameObjectFactory::Add(stage.get(), &tlp)->GetComponent<TestLitPlateComponent>()->SetLight(0, tlc);
}
