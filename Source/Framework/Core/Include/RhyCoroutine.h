#pragma once
#include <ECS/Coroutine/Coroutine.h>
#include <ECS/Coroutine/WaitForSeconds.h>

#define WAIT_FOR_SECONDS( seconds ) \
	{ \
		WaitForSeconds waitForSeconds(seconds); \
		co_yield &waitForSeconds; \
	} 

#define WAIT_FOR_NEXT_FRAME co_yield nullptr;
