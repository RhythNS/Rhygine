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
	// imgui debug gui for changing the light properties.
	ImGui::Begin(("Light " + id), &guiWindowOpen);
	ImGui::DragFloat3("Position", transform->localPosition, 0.1f, -10.0f, 10.0f, "%.3f", 0);
	if (ImGui::DragFloat3("Rotation", direction.m_floats, 0.01f, -1.0f, 1.0f, "%.3f", 0)) {
		transform->localRotation = RhyM::Quat(direction.m_floats[0] * RhyM::PI, direction.m_floats[1] * RhyM::PI, direction.m_floats[2] * RhyM::PI);
	}
	ImGui::ColorEdit4("color", color);
	ImGui::End();
}
