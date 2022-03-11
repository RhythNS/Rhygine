#include <Sound/SoloudComponent.h>
#include <Sound/SoloudModule.h>

SoloudModule* SoloudComponent::GetModuleInstance()
{
	return SoloudModule::instance;
}

SoLoud::Soloud& SoloudComponent::GetSoloud()
{
	return SoloudModule::instance->soloud;
}
