#include "DX11.h"

#include <algorithm>
#include <tracy\Tracy.hpp>

#include "DX11Screen.h"
#include "Windows\WindowsWindow.h"
#include "Windows\WindowsSystem.h"
#include "Debug\Error.h"

Rhygine::DX11::DX11()
{
	ZoneScoped;

    UINT flags = 0;
#ifndef DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    const D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;
	const UINT featureLevelsCount = 1;

	const HRESULT res = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flags,
        &featureLevels,
        featureLevelsCount,
        D3D11_SDK_VERSION,
        &m_device,
        nullptr,
        &m_context
    );
    ASSERT_HRES(res);
}

Rhygine::DX11::~DX11()
{
}

void Rhygine::DX11::OnWindowAdded(Window* t_window)
{
	ZoneScoped;

    WindowsWindow* window = dynamic_cast<WindowsWindow*>(t_window);
    ASSERT_ERROR_MESSAGE(window, "Could not cast WindowsWindow!");

    ASSERT_ERROR_MESSAGE \
    ( \
		m_screens.size() == 0 || std::find_if( \
            m_screens.begin(), \
            m_screens.end(), \
            [&window](const std::unique_ptr<DX11Screen>& x) \
            { \
                return window->GetHandle() == x->m_windowHandle; \
            } \
        ) != m_screens.end(), \
        "Window has already been added!" \
    );


	m_screens.push_back(std::make_unique<DX11Screen>(window->GetHandle()));
    DX11Screen* screen = m_screens.back().get();
	screen->m_width = window->GetWidth();
	screen->m_height = window->GetHeight();

	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.Width = window->GetWidth();
	desc.Height = window->GetHeight();
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	IDXGIDevice2* dxgiDevice = nullptr;
	const HRESULT queryResult = m_device->QueryInterface(__uuidof(IDXGIDevice2), (void**)&dxgiDevice);
	ASSERT_HRES(queryResult);

	IDXGIAdapter* dxgoAdapter = nullptr;
	const HRESULT adapterResult = dxgiDevice->GetAdapter(&dxgoAdapter);
	ASSERT_HRES(adapterResult);

	IDXGIFactory2* dxgiFactory = nullptr;
	const HRESULT parentResult = dxgoAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory);
	ASSERT_HRES(parentResult);

	const HRESULT swapResult = dxgiFactory->CreateSwapChainForHwnd
	(
		reinterpret_cast<IUnknown*>(m_device.Get()),
		window->GetHandle(),
		&desc,
		nullptr, // TODO:
		nullptr,
		&screen->m_swap
	);
	ASSERT_HRES(swapResult);
	
	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	const HRESULT getBufferResult = screen->m_swap->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
	ASSERT_HRES(getBufferResult);
	const HRESULT createTargetResult = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &screen->m_target);
	ASSERT_HRES(createTargetResult);

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
	const HRESULT stencilResult = m_device->CreateDepthStencilState(&stencilDesc, &screen->m_defaultStencilState);
	ASSERT_HRES(stencilResult);

	m_context->OMSetDepthStencilState(screen->m_defaultStencilState.Get(), 1);

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
	const HRESULT textureResult = m_device->CreateTexture2D(&depthTexDesc, nullptr, &depthStencilTex);
	ASSERT_HRES(textureResult);

	// Create the depth stencil.
	D3D11_DEPTH_STENCIL_VIEW_DESC stencilViewDesc = { };
	stencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	stencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencilViewDesc.Texture2D.MipSlice = 0;
	const HRESULT viewResult = m_device->CreateDepthStencilView(depthStencilTex.Get(), &stencilViewDesc, &screen->m_depthStencilView);
	ASSERT_HRES(viewResult);
}

void Rhygine::DX11::OnWindowResized(Window* t_window)
{
	ZoneScoped;

    WindowsWindow* window = dynamic_cast<WindowsWindow*>(t_window);
    ASSERT_ERROR_MESSAGE(window, "Could not cast WindowsWindow!");

    auto iterator = std::find_if(
        m_screens.begin(),
        m_screens.end(),
        [&window](std::unique_ptr<DX11Screen>& x) { return window->GetHandle() == x->m_windowHandle; }
    );

    ASSERT_ERROR_MESSAGE(iterator == m_screens.end(), "Window was not found!");
}

void Rhygine::DX11::OnWindowRemoved(Window* t_window)
{
	ZoneScoped;

    WindowsWindow* window = dynamic_cast<WindowsWindow*>(t_window);
    ASSERT_ERROR_MESSAGE(window, "Could not cast WindowsWindow!");

    auto iterator = std::find_if(
        m_screens.begin(),
        m_screens.end(),
        [&window](std::unique_ptr<DX11Screen>& x) { return window->GetHandle() == x->m_windowHandle; }
    );

    ASSERT_ERROR_MESSAGE(iterator == m_screens.end(), "Window was not found!");

    m_screens.erase(iterator);
}

void Rhygine::DX11::Draw()
{
	ZoneScoped;

	for (auto& screen : m_screens)
	{
		ZoneScopedN("Rhygine::DX11::Draw::Loop");

		m_context->OMSetRenderTargets(1, screen->m_target.GetAddressOf(), screen->m_depthStencilView.Get());
		
		// Clear the target view with the clear color of the scene
		static float clearColor[4] = { 1,0,0,0 };
		m_context->ClearRenderTargetView(screen->m_target.Get(), clearColor);
		// Clear the depth buffer
		m_context->ClearDepthStencilView(screen->m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		// Set the render target
		m_context->OMSetRenderTargets(1, screen->m_target.GetAddressOf(), screen->m_depthStencilView.Get());

		// Set the viewport
		D3D11_VIEWPORT viewport = { 0 };
		viewport.Width = screen->m_width;
		viewport.Height = screen->m_height;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		m_context->RSSetViewports(1, &viewport);
		
		screen->m_swap->Present(1, 0);
	}
}
