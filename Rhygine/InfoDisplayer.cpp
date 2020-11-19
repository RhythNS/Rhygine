#include "InfoDisplayer.h"
#include "Keys.h"
#include "RhyWin.h"
#include "Gameobject.h"
#include "Stage.h"
#include "UIRootElement.h"
#include "UIFitToParentSizer.h"
#include "PresentResources.h"
#include "Window.h"

void InfoDisplayer::Init()
{
	frameDurationFont = GetGameObject()->AddComponent<UIFont>();
	frameDurationFont->SetParent(GetGameObject()->GetStage()->GetUIRoot());
	frameDurationFont->SetSizer(std::make_unique<UIFitToParentSizer>());
	frameDurationFont->Set(&PresentResources::instance->defFont, true, UIFont::VertAlignment::Up, UIFont::HoriAlignment::Right, 0.3f);

	
	infoFont = GetGameObject()->AddComponent<UIFont>();
	infoFont->SetParent(GetGameObject()->GetStage()->GetUIRoot());
	infoFont->SetSizer(std::make_unique<UIFitToParentSizer>());
	infoFont->Set(&PresentResources::instance->defFont, true, UIFont::VertAlignment::Up, UIFont::HoriAlignment::Left, 0.3f);

	infoFont->text =
		"F1 - Show/ Hide info overlay\nF11 - Toggle fullscreen window\nWASDQE - Move camera\nRight mouse button + drag - Rotate camera\nR - Reset camera rotation\nEscape - Close program";
}

void InfoDisplayer::Update()
{
	if (GetKeys()->IsKeyDownThisFrame(VK_F1))
	{
		displaying = !displaying;
		
		frameDurationFont->visible = displaying;
		infoFont->visible = displaying;
	}

	if (!displaying)
		return;

	float currentDuration = Window::GetInstance()->time.LastFrameDuration();

	lastDurations[atDuration++] = currentDuration;

	if (atDuration == 100)
		atDuration = 0;

	float avgDuration = 0;
	for (int i = 0; i < 100; i++)
	{
		avgDuration += lastDurations[i];
	}
	avgDuration /= 100;

	frameDurationFont->text = "Info about how long a frame took to process\nLast duration: " + std::to_string(currentDuration) + "\nAvg (last 100 frames) duration: " + std::to_string(avgDuration);
}
