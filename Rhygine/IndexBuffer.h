#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>

#include "Bindable.h"
#include "RhyException.h"

template <class Index>
class IndexBuffer : public Bindable
{
public:
	IndexBuffer(std::vector<Index> inds) : IndexBuffer<Index>(inds, GetDefaultDescription()) {}
	IndexBuffer(std::vector<Index> inds, D3D11_BUFFER_DESC desc) : size(inds.size())
	{
		desc.ByteWidth = sizeof(Index) * inds.size();
		desc.StructureByteStride = sizeof(Index);
		D3D11_SUBRESOURCE_DATA indexData = { };
		indexData.pSysMem = inds.data();
		THROW_IF_FAILED(GetDevice()->CreateBuffer(&desc, &indexData, &indexPointer));
	}
	void Bind()
	{
		GetContext()->IASetIndexBuffer(indexPointer.Get(), GetFormat(), 0);
	}
	int GetSize()
	{
		return size;
	}
	int slot;
protected:
	D3D11_BUFFER_DESC GetDefaultDescription()
	{
		D3D11_BUFFER_DESC desc = { };
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		return desc;
	}
	virtual DXGI_FORMAT GetFormat() = 0;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexPointer;
	int size;
};

class IndexBufferUC : public IndexBuffer<unsigned char>
{
	using IndexBuffer<unsigned char>::IndexBuffer;
protected:
	DXGI_FORMAT GetFormat() { return DXGI_FORMAT_R8_UINT; }
};

class IndexBufferUS : public IndexBuffer<unsigned short>
{
	using IndexBuffer<unsigned short>::IndexBuffer;
protected:
	DXGI_FORMAT GetFormat() { return DXGI_FORMAT_R16_UINT; }
};

class IndexBufferUI : public IndexBuffer<unsigned int>
{
	using IndexBuffer<unsigned int>::IndexBuffer;
protected:
	DXGI_FORMAT GetFormat() { return DXGI_FORMAT_R32_UINT; }
};
