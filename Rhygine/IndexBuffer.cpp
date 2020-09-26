#include "IndexBuffer.h"
#include "RhyException.h"

template<class Index>
IndexBuffer<Index>::IndexBuffer(std::vector<Index> inds) : IndexBuffer<Index>(inds, GetDefaultDescription())
{
}

template<class Index>
IndexBuffer<Index>::IndexBuffer(std::vector<Index> inds, D3D11_BUFFER_DESC desc) : size(inds.size())
{
	desc.ByteWidth = sizeof(Index) * inds.size();
	desc.StructureByteStride = sizeof(Index);
	D3D11_SUBRESOURCE_DATA indexData = { };
	indexData.pSysMem = inds;
	THROW_IF_FAILED(GetDevice()->CreateBuffer(&desc, &indexData, &indexPointer));
}

template<class Index>
void IndexBuffer<Index>::Bind()
{
	GetContext()->IASetIndexBuffer(indexPointer.Get(), GetFormat(), 0);
}

template<class Index>
int IndexBuffer<Index>::GetSize()
{
	return size;
}

template<class Index>
D3D11_BUFFER_DESC IndexBuffer<Index>::GetDefaultDescription()
{
	D3D11_BUFFER_DESC desc = { };
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	return desc;
}
