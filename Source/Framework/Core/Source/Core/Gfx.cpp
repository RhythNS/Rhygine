#include <Core/Gfx.h>
#include <RhyException.h>
#include <RhyWin.h>
#include <Core/Window.h>
#include <Core/Scene.h>
#include <Input/Keys.h>

#include <d3dcompiler.h>
#include <DirectXMath.h>

Gfx::Gfx(Window* window, int refreshRate, bool vsync) : window(window), refreshRate(refreshRate), vsync(vsync)
{
	instance = this;

	// Create the swap chain description
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 1;
	desc.BufferDesc.RefreshRate.Denominator = refreshRate;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = *window->GetWindowHandle();
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;

	// All required variables for the device and swap chain creation
	IDXGIAdapter* pAdapter = nullptr;
	D3D_DRIVER_TYPE	DriverType = D3D_DRIVER_TYPE_HARDWARE;
	HMODULE	Software = nullptr;
	UINT flags = 0;
#ifndef DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	const D3D_FEATURE_LEVEL* pFeatureLevels = nullptr;
	UINT FeatureLevels = 0;
	UINT SDKVersion = D3D11_SDK_VERSION;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;

	// Create the device, swapchain and context
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

	CreateTargetAndDepth();
}

Gfx::~Gfx()
{
	instance = nullptr;
}

Gfx* Gfx::GetInstance()
{
	return instance;
}

void Gfx::ClearDepth()
{
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Gfx::BeginDraw()
{
	// Clear the target view with the clear color of the scene
	context->ClearRenderTargetView(target.Get(), window->GetCurrentScene()->GetClearColor());
	// Clear the depth buffer
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Set the render target
	context->OMSetRenderTargets(1, target.GetAddressOf(), depthStencilView.Get());

	// Set the viewport
	D3D11_VIEWPORT viewport = { 0 };
	viewport.Width = (float)window->GetWidth();
	viewport.Height = (float)window->GetHeight();
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1, &viewport);
}

void Gfx::DrawIndexed(UINT indexCount, UINT startLocation, INT baseVertexLocation)
{
	context->DrawIndexed(indexCount, startLocation, baseVertexLocation);
}

void Gfx::Draw(UINT vertexCount, UINT startVertexLocation)
{
	context->Draw(vertexCount, startVertexLocation);
}

void Gfx::EndDraw()
{
	// Present it to the screen.
	THROW_IF_FAILED(swap->Present(vsync ? 1 : 0, 0));
}

void Gfx::SetDefaultStencilState()
{
	context->OMSetDepthStencilState(defaultStencilState.Get(), 1);
}

DirectX::XMMATRIX Gfx::GetPerspectiveMatrix()
{
	return DirectX::XMMatrixPerspectiveLH(
		1.0f,
		(float)width / (float)height,
		nearZ,
		farZ
	);
}

void Gfx::OnResize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;

	DXGI_MODE_DESC desc{0};
	desc.Width = newWidth;
	desc.Height = newHeight;
	DXGI_RATIONAL refreshRateRational;
	refreshRateRational.Numerator = 1;
	refreshRateRational.Denominator = refreshRate;
	desc.RefreshRate = refreshRateRational;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	THROW_IF_FAILED(swap->ResizeTarget(&desc));

	target = nullptr;
	depthStencilView = nullptr;
	THROW_IF_FAILED(swap->ResizeBuffers(1, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0));

	CreateTargetAndDepth();
}

inline void Gfx::CreateTargetAndDepth()
{
	// Create the render target to which we are drawing every frame.
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
}

// init for static field.
Gfx* Gfx::instance;
