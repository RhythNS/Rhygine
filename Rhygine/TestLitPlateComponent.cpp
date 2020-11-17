#include "TestLitPlateComponent.h"
#include "RhyBindables.h"
#include "Rhyimgui.h"
#include "TestLight.h"
#include "Window.h"
#include "Gfx.h"
#include "Transform.h"
#include "Drawer.h"
#include "TexLitShader.h"
#include "RotateAround.h"
#include "TextureChanger.h"

void TestLitPlateComponent::Init()
{
	transform = GetGameObject()->GetComponent<Transform>();
}


void TestLitPlateComponent::Update()
{
	ImGui::Begin(("BestPlane" + GetGameObject()->name).c_str(), &guiWindowOpen);
	ImGui::DragFloat3("Position", transform->localPosition.m_floats, 0.1f, -10.0f, 10.0f, "%.3f", 0);
	if (ImGui::DragFloat3("Rotation", direction, 0.01f, -1.0f, 1.0f, "%.3f", 0)) {
		transform->localRotation = RhyM::Quat(direction[0] * RhyM::PI, direction[1] * RhyM::PI, direction[2] * RhyM::PI);
	}
	/*
	bool boxRotates = rotateAround->IsEnabled();
	if (ImGui::Checkbox("Rotate around", &boxRotates))
	{
		if (boxRotates)
			rotateAround->Enable();
		else
			rotateAround->Disable();
	}
	ImGui::DragFloat3("Rotate speed", rotateAround->rotationSpeed, 0.05f, -2.0, 2.0f);
	*/

	ImGui::End();
}


void TestLitPlateComponent::SetLight(TestLightComponent* light)
{
	GetGameObject()->GetComponent<Drawer>()->GetBindable<TexLitShader>()->light = light;
}
