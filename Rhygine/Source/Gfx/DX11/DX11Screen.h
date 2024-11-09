#pragma once

#include "Windows/RhyWindows.h"
#include "Window.h"

namespace Rhygine
{
	struct DX11Screen
	{
	public:
		DX11Screen() = delete;
		DX11Screen(const DX11Screen&) = delete;
		DX11Screen(HWND t_windowHandle);
		
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_target;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_defaultStencilState;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swap;

		HWND m_windowHandle;

		float m_width = 100;
		float m_height = 100;
	};
}
