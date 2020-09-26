#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>

#include "TestCamera.h"

class Window;
class Bindable;

class Gfx
{
	friend class Window;
	friend class Bindable;
public:
	Gfx() = delete;
	Gfx(Window* window);

	void BeginDraw();
	void DrawIndexed(UINT indexCount);
	void EndDraw();

	TestCamera camera;
private:
	Window* window;

	static Gfx* instance;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
};

