#include <Opt/OptNep.h>
#include <ECS/GameObjectFactory.h>
#include <ECS/GameObject.h>
#include <CC/TestLightComponent.h>
#include <Factories/TestLight.h>
#include <Factories/TestNeptuneLoader.h>
#include <ECS/Stage.h>
#include <ECS/Components/Transform.h>
#include <Math/Random.h>

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
