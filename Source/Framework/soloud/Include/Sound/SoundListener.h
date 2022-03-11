#pragma once
#include <ECS/Component.h>

#include <Sound/SoloudComponent.h>

class SoloudModule;
class Transform;

class SoundListener : public SoloudComponent
{
public:
	void Init() override;

	void OnEnabled() override;

	void OnDisabled() override;

	virtual void UpdateListenerParameters(SoLoud::Soloud& soloud);

protected:
	Transform* attachedTransform;

};
