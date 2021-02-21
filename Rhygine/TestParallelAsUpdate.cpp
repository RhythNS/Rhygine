#include "TestParallelAsUpdate.h"
#include "Stage.h"
#include "RhyMath.h"
#include "Random.h"
#include "Gameobject.h"


void TestParallelAsUpdate::Update()
{
	for (int i = 0; i < 1000; i++)
	{
		RhyM::Vec3 p(RhyM::GetRandomFloat(1, 10), RhyM::GetRandomFloat(1, 10), RhyM::GetRandomFloat(1, 10));
		p.Normalize();
	}
}

void TestParallelAsUpdate::AddToStage(Stage* stage)
{
	int components = 100;
	GameObject* current = stage->CreateGameObject();
	while (components > 0)
	{
		current->AddComponent<TestParallelAsUpdate>();
		if (RhyM::GetRandomFloat() > 0.3f)
			current = stage->CreateGameObject();
		--components;
	}
}
