#include "PrimitiveTopolpgy.h"

PrimitiveTopolpgy::PrimitiveTopolpgy(D3D11_PRIMITIVE_TOPOLOGY primitiveTopolpgy) : primitiveTopolpgy(primitiveTopolpgy)
{
}

void PrimitiveTopolpgy::Bind()
{
	GetContext()->IASetPrimitiveTopology(primitiveTopolpgy);
}
