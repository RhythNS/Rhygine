#pragma once
#include "Bindable.h"

class PixShader : public Bindable
{
public:
	PixShader(LPCWSTR fileName, Microsoft::WRL::ComPtr<ID3DBlob> blob);
	void Bind();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shaderPointer;
};

