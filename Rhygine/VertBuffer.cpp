#include "VertBuffer.h"
#include "RhyException.h"

template<class Vertex>
VertBuffer<Vertex>::VertBuffer(std::vector<Vertex> verts) : VertBuffer<Vertex>(verts, GetDefaultDescription())
{
}

template<class Vertex>
VertBuffer<Vertex>::VertBuffer(std::vector<Vertex> verts, D3D11_BUFFER_DESC desc)
{
	desc.ByteWidth = sizeof(Vertex) * verts.size();
	desc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vertData = {  };
	vertData.pSysMem = verts;
	THROW_IF_FAILED(GetDevice()->CreateBuffer(&desc, &vertData, &bufferPointer));
}

template<class Vertex>
void VertBuffer<Vertex>::Bind()
{
	unsigned int strides = sizeof(Vertex);
	unsigned int vertOffset = 0;
	GetContext()->IASetVertexBuffers(slot, 1, bufferPointer.GetAddressOf(), &strides, &vertOffset);
}

template<class Vertex>
D3D11_BUFFER_DESC VertBuffer<Vertex>::GetDefaultDescription()
{
	D3D11_BUFFER_DESC vertBufferDesc = { };
	vertBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertBufferDesc.CPUAccessFlags = 0;
	vertBufferDesc.MiscFlags = 0;
	return vertBufferDesc;
}
