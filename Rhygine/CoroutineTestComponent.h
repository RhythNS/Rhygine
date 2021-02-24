#pragma once
#include "Component.h"
#include "Updatable.h"
#include "RhyCoroutine.h"
#include <memory>

class CoroutineTestComponent : public Component, public Updatable
{
public:
	void Init();
	void Update();
	void OnFinish();
private:
	Yielder Counter();
	std::weak_ptr<Coroutine> activeCoroutine{};
};
