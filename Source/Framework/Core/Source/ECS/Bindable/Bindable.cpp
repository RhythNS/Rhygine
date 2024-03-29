#include <ECS/Bindable/Bindable.h>
#include <Core/Window.h>
#include <Core/Gfx.h>
#include <ECS/Bindable/Drawer.h>
#include <ECS/Components/Transform.h>

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

DirectX::XMMATRIX Bindable::GetWorldMatrix(Drawer* drawer)
{
    return drawer->GetTransform()->GetWorldMatrix();
}

DirectX::XMMATRIX Bindable::GetPerspectiveMatrix(Drawer* drawer)
{
    return drawer->GetTransform()->GetPerspectiveMatrix();
}

DirectX::XMMATRIX Bindable::GetLocalMatrix(Drawer* drawer)
{
    return drawer->GetTransform()->GetLocalMatrix();
}
