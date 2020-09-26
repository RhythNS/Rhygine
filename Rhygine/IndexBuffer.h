#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>

#include "Bindable.h"

template <class Index>
class IndexBuffer : public Bindable
{
public:
	IndexBuffer(std::vector<Index> inds);
	IndexBuffer(std::vector<Index> inds, D3D11_BUFFER_DESC desc);
	void Bind();

	int GetSize();
	int slot;
protected:
	D3D11_BUFFER_DESC GetDefaultDescription();
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
