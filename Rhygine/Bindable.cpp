#include "Bindable.h"
#include "Window.h"
#include "Gfx.h"

Microsoft::WRL::ComPtr<ID3D11Device> Bindable::GetDevice()
{
    return Gfx::instance->device;
}

Microsoft::WRL::ComPtr<IDXGISwapChain> Bindable::GetSwap()
{
    return Gfx::instance->swap;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> Bindable::GetContext()
{
    return Gfx::instance->context;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Bindable::GetTarget()
{
    return Gfx::instance->target;
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> Bindable::GetDepthStencilView()
{
    return Gfx::instance->depthStencilView;
}

