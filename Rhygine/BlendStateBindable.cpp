#include "BlendStateBindable.h"
#include "RhyException.h"

BlendStateBindable::BlendStateBindable() : BlendStateBindable(GetDefaultDescription())
{ }

BlendStateBindable::BlendStateBindable(D3D11_BLEND_DESC desc)
{
	THROW_IF_FAILED(GetDevice()->CreateBlendState(&desc, &blendState));
}

void BlendStateBindable::Bind()
{
	GetContext()->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);
}

void BlendStateBindable::UnBind()
{
	GetContext()->OMSetBlendState(NULL, nullptr, 0xffffffff);
}

D3D11_BLEND_DESC BlendStateBindable::GetDefaultDescription()
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	return desc;
}
