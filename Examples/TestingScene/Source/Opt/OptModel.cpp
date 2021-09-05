#include <Opt/OptModel.h>
#include <Factories/TestLight.h>
#include <ECS/GameObject.h>
#include <ECS/GameObjectFactory.h>
#include <CC/TestLightComponent.h>
#include <ECS/Stage.h>
#include <Factories/TestBasicLit.h>
#include <Components/RotateAround.h>
#include <Math/Random.h>

void OptModel::Init()
{
	Stage* stage = GetGameObject()->GetStage();

	TestLight tl;
	TestLightComponent* tlc = GameObjectFactory::Add(stage, &tl)->GetComponent<TestLightComponent>();

	tex = std::make_unique<Texture>("PresentScene\\carpet.jpg", 0);

	for (int i = 0; i < 100; i++)
	{
		TestBasicLit tbl = TestBasicLit(tex.get(), tlc);
		GameObject* litCarpet = GameObjectFactory::Add(stage, &tbl);
		RhyM::Vec3 randomPos = RhyM::Vec3(RhyM::GetRandomFloat() * 10 -5, RhyM::GetRandomFloat() * 10 -5, RhyM::GetRandomFloat() * 10 -5);
		//OutputDebugString((std::to_string(randomPosx) + " " + std::to_string(randomPos.y)).c_str());
		litCarpet->GetComponent<Transform>()->localPosition = randomPos;
		litCarpet->GetComponent<Transform>()->localScale.Set(1.0f, 1.0f, 1.0f);
		litCarpet->RemoveComponent<RotateAround>();
	}

}
