#include "Gfx.h"
#include "Debug/Error.h"

Rhygine::Gfx* Rhygine::Gfx::s_instance = nullptr;

Rhygine::Gfx::Gfx()
{
	ASSERT_ERROR_MESSAGE(!s_instance, "Gfx already set!");

	s_instance = this;
}

Rhygine::Gfx::~Gfx()
{
	ASSERT_ERROR_MESSAGE(s_instance, "Gfx already delted!");

	s_instance = nullptr;
}

Rhygine::Gfx* Rhygine::Gfx::GetInstance()
{
	return s_instance;
}
