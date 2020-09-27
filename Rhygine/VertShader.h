#pragma once
#include "Bindable.h"

class VertShader : public Bindable
{
public:
	VertShader(LPCWSTR fileName);
	void Bind();
	ID3DBlob* GetBlob();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shaderPointer;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
};

