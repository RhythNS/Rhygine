#pragma once
#include "Bindable.h"
#include "UnBindable.h"

class BlendStateBindable : public Bindable, public UnBindable
{
public:
	BlendStateBindable();
	BlendStateBindable(D3D11_BLEND_DESC desc);

	void Bind();
	void UnBind();
private:
	D3D11_BLEND_DESC GetDefaultDescription();

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
};
