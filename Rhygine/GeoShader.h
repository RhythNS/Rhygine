#pragma once
#include "Bindable.h"

class GeoShader : public Bindable
{
public:
	GeoShader(LPCWSTR fileName);
	void Bind();
	ID3DBlob* GetBlob();
private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> shaderPointer;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
};

