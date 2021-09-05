#pragma once
#include <RhyWin.h>

class IWin32MessageHandler
{
public:
	virtual bool ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
