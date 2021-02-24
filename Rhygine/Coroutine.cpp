#include "Coroutine.h"
#include "Gameobject.h"
#include <assert.h>

Coroutine::Coroutine(Yielder yielder, GameObject* onObject, std::function<void()> onFinish) : yielder(yielder), onFinish(onFinish), onObject(onObject)
{
	assert(yielder.h_);
	currentYield = &this->yielder.h_.promise();
}

Coroutine::~Coroutine()
{
	if (yielder.h_)
		yielder.h_.destroy();
}

void Coroutine::Stop(bool invokeOnFinish)
{
	if (invokeOnFinish && onFinish)
		onFinish();

	onObject->RemoveCoroutine(this);
}

bool Coroutine::operator==(const Coroutine& other)
{
	return yielder.h_ == other.yielder.h_;
}

bool Coroutine::Update()
{
	// Is a yield executing and is not yet finished?
	if (currentYield->value_ && !currentYield->value_->IsDone())
		return false;

	// Is the coroutine done?
	if (yielder.h_.done())
	{
		if (onFinish)
			onFinish();
		return true;
	}

	yielder.h_();

	return false;
}
