#include "Sampler.h"
#include "RhyException.h"

Sampler::Sampler(int slot) : Sampler(slot, GetDefaultDescription())
{
}

Sampler::Sampler(int slot, D3D11_SAMPLER_DESC desc) : slot(slot)
{
	THROW_IF_FAILED(GetDevice()->CreateSamplerState(&desc, &samplerPointer));
}

void Sampler::Bind()
{
	GetContext()->PSSetSamplers(slot, 1, samplerPointer.GetAddressOf());
}

ID3D11SamplerState* Sampler::Get()
{
	return samplerPointer.Get();
}

D3D11_SAMPLER_DESC Sampler::GetDefaultDescription()
{
	D3D11_SAMPLER_DESC desc = { };
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	return desc;
}
