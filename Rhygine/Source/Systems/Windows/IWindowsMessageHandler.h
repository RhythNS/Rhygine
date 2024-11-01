#pragma once

#include "Windows.h"

namespace Rhygine
{
	class IWindowsMessageHandler
	{
	public:
		virtual bool ProcessMessage(HWND t_window, UINT t_message, WPARAM t_w_param, LPARAM t_l_param) = 0;
	};
}
