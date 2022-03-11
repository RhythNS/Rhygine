#include <Sound/SoloudModule.h>

#include <Sound/SoundListener.h>
#include <Core/Log.h>

void SoloudModule::Setup()
{
	soloud.init(SoLoud::Soloud::LEFT_HANDED_3D & SoLoud::Soloud::CLIP_ROUNDOFF);

	instance = this;
}

SoloudModule::~SoloudModule()
{
	soloud.deinit();
}

void SoloudModule::PreDrawAfterUpdate()
{
	if (currentListeners.size() < 1)
		LOG_WARN("There is no SoundListener active! The 3d sound will not update!");
	else
	{
		currentListeners[0]->UpdateListenerParameters(soloud);
		if (currentListeners.size() > 1)
			LOG_WARN("There is more than one SoundListener active!");
	}

	soloud.update3dAudio();
}

void SoloudModule::AddListener(SoundListener* toAdd)
{
	currentListeners.push_back(toAdd);
}

void SoloudModule::RemoveListener(SoundListener* toRemove)
{
	std::erase(currentListeners, toRemove);
}

SoloudModule* SoloudModule::GetInstance()
{
	return instance;
}

SoLoud::Soloud& SoloudModule::GetSoloud()
{
	return soloud;
}

SoloudModule* SoloudModule::instance;
