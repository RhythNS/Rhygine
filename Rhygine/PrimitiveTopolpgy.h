#pragma once
#include "Bindable.h"

class PrimitiveTopolpgy : public Bindable
{
public:
	PrimitiveTopolpgy(D3D11_PRIMITIVE_TOPOLOGY primitiveTopolpgy);
	void Bind();
private:
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopolpgy;
};

