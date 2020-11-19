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
	ImGui::ColorEdit4("color", color);
	ImGui::End();
}
