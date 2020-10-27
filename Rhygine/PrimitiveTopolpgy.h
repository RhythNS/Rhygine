#pragma once
#include "Bindable.h"

class PrimitiveTopology : public Bindable
{
public:
	PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopolpgy);
	void Bind();
	D3D11_PRIMITIVE_TOPOLOGY* GetTopology();
private:
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
};

