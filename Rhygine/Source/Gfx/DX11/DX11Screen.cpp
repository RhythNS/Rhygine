#include "DX11Screen.h"

Rhygine::DX11Screen::DX11Screen(HWND t_windowHandle) : m_windowHandle(t_windowHandle)
{
	/*
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//desc.BufferDesc.RefreshRate.Numerator = 1;
	// desc.BufferDesc.RefreshRate.Denominator = refreshRate;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = m_windowHandle;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;


	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	THROW_IF_FAILED(swap->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	THROW_IF_FAILED(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &target));

	// Create the stencil description.
	D3D11_DEPTH_STENCIL_DESC stencilDesc = { 0 };
	stencilDesc.DepthEnable = true;
	stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	stencilDesc.StencilEnable = true;
	stencilDesc.StencilReadMask = 0xFF;
	stencilDesc.StencilWriteMask = 0xFF;

	stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the stencil with the description.
	THROW_IF_FAILED(device->CreateDepthStencilState(&stencilDesc, &defaultStencilState));

	context->OMSetDepthStencilState(defaultStencilState.Get(), 1);

	// Create the depth texture for the depth buffer.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTex;
	D3D11_TEXTURE2D_DESC depthTexDesc = { 0 };
	depthTexDesc.Width = window->GetWidth();
	depthTexDesc.Height = window->GetHeight();
	depthTexDesc.MipLevels = 1;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTexDesc.SampleDesc.Count = 1;
	depthTexDesc.SampleDesc.Quality = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	THROW_IF_FAILED(device->CreateTexture2D(&depthTexDesc, nullptr, &depthStencilTex));

	// Create the depth stencil.
	D3D11_DEPTH_STENCIL_VIEW_DESC stencilViewDesc = { };
	stencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	stencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencilViewDesc.Texture2D.MipSlice = 0;
	THROW_IF_FAILED(device->CreateDepthStencilView(depthStencilTex.Get(), &stencilViewDesc, &depthStencilView));

	context->OMSetRenderTargets(1, target.GetAddressOf(), depthStencilView.Get());

	*/
}
