#include <Components/CoroutineTestComponent.h>
#include <ECS/GameObject.h>
#include <Input/Keys.h>
#include <RhyWin.h>

#include <string>
#include <functional>

void CoroutineTestComponent::Init()
{
	// activeCoroutine = GetGameObject()->StartCoroutine(Counter(), std::bind(&CoroutineTestComponent::OnFinish, this));
	activeCoroutine = StartCoroutine(Counter());
}

void CoroutineTestComponent::Update()
{
	if (!activeCoroutine.expired() && GetKeys()->IsKeyDown('X'))
	{
		activeCoroutine.lock()->Stop(true);
	}
}

void CoroutineTestComponent::OnFinish()
{
	OutputDebugString("Yay end?!\n");
}

Yielder CoroutineTestComponent::Counter()
{
	for (int i = 0; i < 3; i++)
	{
		OutputDebugString(("Yay: " + std::to_string(i) + "\n").c_str());
		WAIT_FOR_SECONDS(1)
		OutputDebugString("Frame\n");
		WAIT_FOR_NEXT_FRAME
		OutputDebugString("Frame\n");
	}
}
