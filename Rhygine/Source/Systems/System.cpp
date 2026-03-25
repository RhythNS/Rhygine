#include "System.h"

#include <string>

#include "Debug/Logger.h"
#include "Debug/Error.h"

Rhygine::System* Rhygine::System::s_instance = nullptr;

Rhygine::System::System(Config& t_config)
{
	ASSERT_ERROR_MESSAGE(s_instance == nullptr, "System is already set!");

	s_instance = this;
}

Rhygine::System::~System()
{
	s_instance = nullptr;
}

Rhygine::System* Rhygine::System::GetInstance()
{
	return s_instance;
}
