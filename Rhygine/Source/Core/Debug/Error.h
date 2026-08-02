#pragma once
#include "Debug/Logger.h"
#include "Debug/StackTrace.h"

#ifdef _MSC_VER
#define STOP_EXECUTION __debugbreak()
#else
#include <csignal>
#define STOP_EXECUTION raise(SIGTRAP)
#endif

#define STOP_EXECUTION_MESSAGE(message) \
	{ \
		LOG_ERROR_STACK(message); \
		STOP_EXECUTION;  \
	} 

#define STOP_EXECUTION_NOT_IMPLEMENTED STOP_EXECUTION_MESSAGE("#__method__ not implemented")

#define ASSERT_ERROR_MESSAGE(cond, message) \
	if (!(cond)) \
	{ \
		LOG_ERROR_STACK(message); \
		STOP_EXECUTION; \
	} 

#define ASSERT_ERROR(cond) \
	ASSERT_ERROR_MESSAGE(cond, "#cond  asserted to false!")
