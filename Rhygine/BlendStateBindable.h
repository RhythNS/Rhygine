#pragma once
#include "Bindable.h"
#include "UnBindable.h"

class BlendStateBindable : public Bindable, public UnBindable
{
public:
	void Bind();
	void UnBind();
	void Init();
private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
};
