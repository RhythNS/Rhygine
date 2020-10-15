#include "TestLightComponent.h"
#include "GameObject.h"
#include "Rhyimgui.h"
#include "Transform.h"
#include "RhyMath.h"

void TestLightComponent::Init()
{
	guiWindowOpen = true;
	transform = GetGameObject()->GetComponent<Transform>();
}

void TestLightComponent::Update()
{
	ImGui::Begin(("Light " + id), &guiWindowOpen);
	ImGui::DragFloat3("Position", &transform->position.x, 0.1f, -10.0f, 10.0f, "%.3f", 0);
	if (ImGui::DragFloat3("Rotation", &direction.x, 0.01f, -1.0f, 1.0f, "%.3f", 0)) {
		transform->rotation = RhyM::Quat(direction.x * RhyM::PI, direction.y * RhyM::PI, direction.z * RhyM::PI);
	}
	ImGui::End();
}
