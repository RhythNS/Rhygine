#pragma once
#include "RhyWin.h"
#include "Bindable.h"

#include <vector>

/// <summary>
/// Interface for getting the size of the vertex buffer.
/// </summary>
class VertBufferAmount
{
public:
	virtual UINT* GetSize() = 0;
};

/// <summary>
/// Represents a vertex buffer.
/// </summary>
/// <typeparam name="Vertex">The type of vertex.</typeparam>
template <class Vertex>
class VertBuffer : public Bindable, public VertBufferAmount
{
public:
	/// <summary>
	/// Creates a vertex buffer with the default descriptions.
	/// </summary>
	/// <param name="verts">The verticies.</param>
	/// <param name="slot">The slot to where the vertex buffer will be bound to.</param>
	VertBuffer(std::vector<Vertex> verts, int slot) : VertBuffer<Vertex>(verts, GetDefaultDescription(), slot) {}

	/// <summary>
	/// Creates a vertex buffer with a custom descriptions.
	/// </summary>
	/// <param name="verts">The verticies.</param>
	/// <param name="desc">The custom description.</param>
	/// <param name="slot">The slot to where the vertex buffer will be bound to.</param>
	VertBuffer(std::vector<Vertex> verts, D3D11_BUFFER_DESC desc, int slot) : slot(slot)
	{
		amount = (UINT)verts.size();
		desc.ByteWidth = (UINT)(sizeof(Vertex) * verts.size());
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

	UINT* GetSize()
	{
		return &amount;
	}
	
	int slot;
	UINT amount;

protected:
	/// <summary>
	/// Gets the default description
	/// </summary>
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