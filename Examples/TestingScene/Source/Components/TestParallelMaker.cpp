#include <Components/TestParallelMaker.h>
#include <ECS/Stage.h>
#include <Math/Random.h>
#include <ECS/GameObject.h>
#include <RhyMath.h>

void TestParallelMaker::ParallelUpdate()
{
	for (int i = 0; i < 1000; i++)
	{
		RhyM::Vec3 p(RhyM::GetRandomFloat(1, 10), RhyM::GetRandomFloat(1, 10), RhyM::GetRandomFloat(1, 10));
		p.Normalize();
	}
}

void TestParallelMaker::AddToStage(Stage* stage)
{
	int components = 100;
	GameObject* current = stage->CreateGameObject();
	while (components > 0)
	{
		current->AddComponent<TestParallelMaker>();
		if (RhyM::GetRandomFloat() > 0.3f)
			current = stage->CreateGameObject();
		--components;
	}
}
