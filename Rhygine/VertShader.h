#pragma once
#include "Bindable.h"

class VertShader : public Bindable
{
public:
	VertShader(LPCWSTR fileName, Microsoft::WRL::ComPtr<ID3DBlob> blob);
	void Bind();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shaderPointer;
};

