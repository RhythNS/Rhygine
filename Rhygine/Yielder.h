#pragma once
#include <coroutine>
#include "YieldInstruction.h"

struct Yielder
{
	struct promise_type 
	{
		YieldInstruction* value_;
		Yielder get_return_object() 
		{
			return 
			{
			  .h_ = std::coroutine_handle<promise_type>::from_promise(*this)
			};
		}
		
		std::suspend_always initial_suspend() { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void unhandled_exception() {}
		
		std::suspend_always yield_value(YieldInstruction* value) 
		{
			value_ = value;
			return {};
		}
		
		void return_void() {}
	};

	std::coroutine_handle<promise_type> h_;
};
