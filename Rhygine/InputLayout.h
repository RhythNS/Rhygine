#pragma once
#include "Bindable.h"

#include <vector>

class InputLayout : public Bindable
{
public:
	InputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> desc, ID3DBlob* blob);
	void Bind();
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layoutPointer;
};

