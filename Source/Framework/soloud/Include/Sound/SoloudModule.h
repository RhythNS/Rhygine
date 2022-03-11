#pragma once
#include <Core/Module/Module.h>

#include <soloud.h>
#include <vector>

class SoloudComponent;
class SoundListener;

class SoloudModule : public Module
{
	friend class SoloudComponent;

public:
	virtual ~SoloudModule() override;

	virtual void Setup() override;
	virtual void PreDrawAfterUpdate() override;

	void AddListener(SoundListener* toAdd);
	void RemoveListener(SoundListener* toRemove);

protected:
	static SoloudModule* GetInstance();
	SoLoud::Soloud& GetSoloud();

private:
	static SoloudModule* instance;
	SoLoud::Soloud soloud;
	std::vector<SoundListener*> currentListeners{};
};
