#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>

class Gfx
{
public:
	Gfx() = delete;
	Gfx(HWND windowHandle);

	void BeginDraw();
	void EndDraw();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
};

