#include <ECS/Bindable/PrimitiveTopolpgy.h>

PrimitiveTopology::PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology) : primitiveTopology(primitiveTopology)
{
}

void PrimitiveTopology::Bind()
{
	GetContext()->IASetPrimitiveTopology(primitiveTopology);
}

D3D11_PRIMITIVE_TOPOLOGY* PrimitiveTopology::GetTopology()
{
	return &primitiveTopology;
}
