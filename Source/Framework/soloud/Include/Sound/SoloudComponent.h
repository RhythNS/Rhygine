#pragma once
#include <ECS/Component.h>

#include <soloud.h>

class SoloudModule;

class SoloudComponent : public Component
{
protected:
	SoloudModule* GetModuleInstance();
	SoLoud::Soloud& GetSoloud();

};
