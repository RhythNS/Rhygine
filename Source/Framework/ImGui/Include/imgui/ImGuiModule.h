#pragma once
#include <Core/Module/Module.h>
#include <Core/Module/IWin32MessageHandler.h>

class ImGuiModule : public Module, IWin32MessageHandler
{
public:
	virtual ~ImGuiModule() override;
	bool ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

protected:
	virtual void Setup() override;
	virtual void StartOfFramePreUpdate() override;
	virtual void LateDraw() override;
};
