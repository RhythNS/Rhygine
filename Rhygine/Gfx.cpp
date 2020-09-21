#include "Gfx.h"
#include "RhyException.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

Gfx::Gfx(HWND windowHandle)
{
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = windowHandle;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;

	IDXGIAdapter* pAdapter = nullptr;
	D3D_DRIVER_TYPE	DriverType = D3D_DRIVER_TYPE_HARDWARE;
	HMODULE	Software = nullptr;
	UINT flags = 0u;
#ifndef DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	const D3D_FEATURE_LEVEL* pFeatureLevels = nullptr;
	UINT FeatureLevels = 0;
	UINT SDKVersion = D3D11_SDK_VERSION;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;

	THROW_IF_FAILED(D3D11CreateDeviceAndSwapChain(
		pAdapter,
		DriverType,
		Software,
		flags,
		pFeatureLevels,
		FeatureLevels,
		SDKVersion,
		&desc,
		&swap,
		&device,
		pFeatureLevel,
		&context
	));

	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	THROW_IF_FAILED(swap->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	THROW_IF_FAILED(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &target));
}

void Gfx::BeginDraw()
{
	const float clear[] = { 0,0,1,1 };
	context->ClearRenderTargetView(target.Get(), clear);
}

void Gfx::EndDraw()
{
	THROW_IF_FAILED(swap->Present(1u, 0u));
}
