#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>

#include "Bindable.h"

template <class Vertex>
class VertBuffer : public Bindable
{
public:
	VertBuffer(std::vector<Vertex> verts, int slot) : VertBuffer<Vertex>(verts, GetDefaultDescription(), slot)
	{
	}
	VertBuffer(std::vector<Vertex> verts, D3D11_BUFFER_DESC desc, int slot) : slot(slot)
	{
		desc.ByteWidth = sizeof(Vertex) * verts.size();
		desc.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA vertData = {  };
		vertData.pSysMem = verts.data();
		THROW_IF_FAILED(GetDevice()->CreateBuffer(&desc, &vertData, &bufferPointer));
	}
	void Bind()
	{
		unsigned int strides = sizeof(Vertex);
		unsigned int vertOffset = 0;
		GetContext()->IASetVertexBuffers(slot, 1, bufferPointer.GetAddressOf(), &strides, &vertOffset);
	}
	
	int slot;
protected:
	D3D11_BUFFER_DESC GetDefaultDescription()
	{
		D3D11_BUFFER_DESC vertBufferDesc = { };
		vertBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertBufferDesc.CPUAccessFlags = 0;
		vertBufferDesc.MiscFlags = 0;
		return vertBufferDesc;
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> bufferPointer;
};