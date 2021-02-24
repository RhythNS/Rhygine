#pragma once
#include "Coroutine.h"
#include "WaitForSeconds.h"

#define WAIT_FOR_SECONDS( seconds ) \
	{ \
		WaitForSeconds waitForSeconds(seconds); \
		co_yield &waitForSeconds; \
	} 

#define WAIT_FOR_NEXT_FRAME co_yield nullptr;
