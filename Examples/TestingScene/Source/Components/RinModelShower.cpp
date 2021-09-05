#include <Components/RinModelShower.h>
#include <Rhyimgui.h>
#include <string>

void RinModelShower::Set(std::vector<Drawer*> drawers)
{
	this->drawers = drawers;
}

void RinModelShower::Update()
{
	ImGui::Begin("Rin");
	for (size_t i = 0; i < drawers.size(); i++)
	{
		bool enabled = drawers[i]->IsEnabled();
		bool newEnabled = enabled;
		ImGui::Checkbox(std::to_string(i).c_str(), &newEnabled);
		if (enabled != newEnabled)
		{
			if (newEnabled)
				drawers[i]->Enable();
			else
				drawers[i]->Disable();
		}
	}
	ImGui::End();
}
