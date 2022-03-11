#include <Sound/SoundListener.h>

#include <Sound/SoloudModule.h>
#include <ECS/Components/Transform.h>
#include <ECS/GameObject.h>
#include <Core/Log.h>

void SoundListener::Init()
{
	attachedTransform = GetGameObject()->GetComponent<Transform>();

	if (!attachedTransform)
		LOG_ERROR("The SoundListener GameObject did not have a Transform attached. The Listener will therefore not update its position!");
}

void SoundListener::OnEnabled()
{
	GetModuleInstance()->AddListener(this);
}

void SoundListener::OnDisabled()
{
	GetModuleInstance()->RemoveListener(this);
}

void SoundListener::UpdateListenerParameters(SoLoud::Soloud& soloud)
{
	if (!attachedTransform)
		return;

	RhyM::Vec3 pos = attachedTransform->GetWorldPosition();
	RhyM::Vec3 forward = attachedTransform->GetForward();

	soloud.set3dListenerPosition(pos.x, pos.y, pos.z);
	soloud.set3dListenerAt(forward.x, forward.y, forward.z);
}
