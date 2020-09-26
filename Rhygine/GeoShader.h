#pragma once
#include "Bindable.h"

class GeoShader : public Bindable
{
public:
	GeoShader(LPCWSTR fileName);
	void Bind();
private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> shaderPointer;
};

