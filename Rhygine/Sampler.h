#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(int slot);
	Sampler(int slot, D3D11_SAMPLER_DESC desc);
	void Bind();
	int slot;
private:
	D3D11_SAMPLER_DESC GetDefaultDescription();
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerPointer;
};

