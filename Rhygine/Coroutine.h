#pragma once
#include <functional>
#include <memory>
#include "Yielder.h"
#include "YieldInstruction.h"

class GameObject;

/// <summary>
/// Runs code before the gameobject has been updated. Enables the use of co_yield YieldInstruction
/// to temporarily stop execution. For example, the YieldInstruction WaitForSeconds can be used to
/// delay execution by given amount of seconds. The coroutine is removed from the gameobject when the
/// Yielder function finishes or the user calls Stop on this Coroutine.
/// </summary>
class Coroutine
{
	friend class GameObject;
public:
	~Coroutine();
	
	/// <summary>
	/// Stops the coroutine. 
	/// </summary>
	/// <param name="invokeOnFinish">Wheter the onFinish method should be invoked or not.</param>
	void Stop(bool invokeOnFinish = false);

	bool operator ==(const Coroutine& other);

private:
	/// <summary>
	/// Private constructor that only GameObject should be able to call.
	/// </summary>
	/// <param name="yielder">The coroutine code that is run.</param>
	/// <param name="onObject">Which gameobject this coroutine is supposed to be run.</param>
	/// <param name="onFinish">Function that is called when the coroutine has finished.</param>
	Coroutine(Yielder yielder, GameObject* onObject, std::function<void()> onFinish);

	/// <summary>
	/// Continues exectuion of the coroutine.
	/// </summary>
	/// <returns>Wheter it finished or not.</returns>
	bool Update();

	/// <summary>
	/// The GameObject that this coroutine is on.
	/// </summary>
	GameObject* onObject;
	/// <summary>
	/// The current YieldInstruction.
	/// </summary>
	Yielder::promise_type* currentYield;
	/// <summary>
	/// Struct that has the coroutine pointer inside it.
	/// </summary>
	Yielder yielder;
	/// <summary>
	/// The method that is called when the coroutine finishes.
	/// </summary>
	std::function<void()> onFinish;
};
