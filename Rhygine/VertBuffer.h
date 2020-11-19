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
	/// <summary>
	/// Gets the size of the vertex buffer.
	/// </summary>
	/// <returns></returns>
	virtual UINT GetSize() = 0;
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

	/// <summary>
	/// Creates a vert buffer as an dynamic buffer, meaning that the vers can be updated via
	/// UpdateVerts()
	/// </summary>
	/// <param name="placeHolderFill">Vertex is used as a placeholder to fill the buffer with.</param>
	/// <param name="maxSize">The max size of the buffer.</param>
	/// <param name="slot">The slot to where the buffer is bound to.</param>
	VertBuffer(Vertex placeHolderFill, int maxSize, int slot) : amount(0), slot(slot)
	{
		std::vector<Vertex> verts( maxSize, placeHolderFill );
		D3D11_BUFFER_DESC desc = GetUpdatableDescription();
		desc.ByteWidth = (UINT)(sizeof(Vertex) * maxSize);
		desc.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA vertData = { };
		vertData.pSysMem = verts.data();
		THROW_IF_FAILED(GetDevice()->CreateBuffer(&desc, &vertData, &bufferPointer));
	}

	/// <summary>
	/// Updates the verts of the vert buffer. This only works if the buffer was
	/// created with a correct description which allows updating.
	/// </summary>
	/// <param name="verts">The new verts.</param>
	void UpdateVerts(std::vector<Vertex>& verts)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		THROW_IF_FAILED(GetContext()->Map(bufferPointer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource));
		memcpy(resource.pData, verts.data(), sizeof(Vertex) * verts.size());
		GetContext()->Unmap(bufferPointer.Get(), 0);

		amount = static_cast<UINT>(verts.size());
	}

	void Bind()
	{
		unsigned int strides = sizeof(Vertex);
		unsigned int vertOffset = 0;
		GetContext()->IASetVertexBuffers(slot, 1, bufferPointer.GetAddressOf(), &strides, &vertOffset);
	}

	UINT GetSize()
	{
		return amount;
	}

	/// <summary>
	/// Gets a pointer to the vert buffer.
	/// </summary>
	ID3D11Buffer* Get()
	{
		bufferPointer.Get();
	}

	/// <summary>
	/// Gets a description which allows the updating of verts.
	/// </summary>
	static D3D11_BUFFER_DESC GetUpdatableDescription()
	{
		D3D11_BUFFER_DESC desc = { };
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		return desc;
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
