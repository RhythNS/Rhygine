#include <ECS/Bindable/StencilStates.h>
#include <RhyException.h>
#include <Core/Gfx.h>

StencilStates::StencilStates(D3D11_DEPTH_STENCIL_DESC desc)
{
    THROW_IF_FAILED(GetDevice()->CreateDepthStencilState(&desc, &stencilState));
}

void StencilStates::Bind()
{
	GetContext()->OMSetDepthStencilState(stencilState.Get(), 1);
}

void StencilStates::UnBind()
{
	Gfx::GetInstance()->SetDefaultStencilState();
}

D3D11_DEPTH_STENCIL_DESC StencilStates::GetNoDepthTest()
{
	D3D11_DEPTH_STENCIL_DESC desc{ 0 };
    desc.DepthEnable = FALSE;
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    desc.StencilEnable = FALSE;
    desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

    desc.BackFace = desc.FrontFace;
    return desc;
}
