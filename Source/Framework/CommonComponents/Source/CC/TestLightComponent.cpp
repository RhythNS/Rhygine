#include <CC/TestLightComponent.h>
#include <ECS/GameObject.h>
#include <ECS/Components/Transform.h>

#include <RhyMath.h>
#include <Rhyimgui.h>

void TestLightComponent::Init()
{
	guiWindowOpen = true;
	transform = GetGameObject()->GetComponent<Transform>();
}


void TestLightComponent::Update()
{
	// imgui debug gui for changing the light properties.
	ImGui::Begin(("Light " + id), &guiWindowOpen);
	ImGui::DragFloat3("Position", transform->localPosition.data, 0.1f, -10.0f, 10.0f, "%.3f", 0);
	ImGui::ColorEdit4("color", color);
	ImGui::End();
}
