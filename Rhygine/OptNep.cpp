#include "OptNep.h"
#include "GameObjectFactory.h"
#include "Gameobject.h"
#include "TestLightComponent.h"
#include "TestLight.h"
#include "TestNeptuneLoader.h"
#include "Stage.h"
#include "Transform.h"
#include "Random.h"

void OptNep::Init()
{
	Stage* stage = GetGameObject()->GetStage();

	TestLight tl;
	TestLightComponent* tlc = GameObjectFactory::Add(stage, &tl)->GetComponent<TestLightComponent>();

	TestNeptuneLoader nepLoader = TestNeptuneLoader(tlc);
	for (int i = 0; i < 100; i++)
	{
		GameObjectFactory::Add(stage, &nepLoader)->GetComponent<Transform>()->localPosition 
			= RhyM::Vec3(RhyM::GetRandomFloat() * 10 - 5, RhyM::GetRandomFloat() * 10 - 5, RhyM::GetRandomFloat() * 10 - 5);;
	}

}
