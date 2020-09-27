#pragma once
#include "Bindable.h"

class PixShader : public Bindable
{
public:
	PixShader(LPCWSTR fileName);
	void Bind();
	ID3DBlob* GetBlob();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shaderPointer;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
};

