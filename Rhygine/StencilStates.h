#pragma once
#include "Bindable.h"
#include "UnBindable.h"

class StencilStates : public Bindable, public UnBindable
{
public:
	StencilStates(D3D11_DEPTH_STENCIL_DESC desc);
	void Bind();
	void UnBind();

	static D3D11_DEPTH_STENCIL_DESC GetNoDepthTest();
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> stencilState;
};
