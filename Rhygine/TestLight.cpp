#include "TestLight.h"
#include "RhyBindables.h"
#include "Rhyimgui.h"

TestLight::TestLight(int id, float* dir[3]) : id(id)
{
	direction[0] = *dir[0];
	direction[1] = *dir[1];
	direction[2] = *dir[2];
}

void TestLight::Init()
{
}

void TestLight::Update()
{
	ImGui::Begin(("Light " + id), &guiWindowOpen);
	ImGui::DragFloat3("Position", &transform.position.x, 0.1f, -10.0f, 10.0f, "%.3f", 0);
	ImGui::DragFloat3("Rotation", &transform.position.x, 1.0f, 0.0f, 360.0f, "%.3f", 0);
	ImGui::End();
}
