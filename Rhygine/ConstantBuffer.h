#pragma once
#include "Bindable.h"
#include "RhyWin.h"
#include "RhyException.h"
#include "Updatable.h"

#include <vector>

/// <summary>
/// Representation of a custom constant buffer.
/// </summary>
/// <typeparam name="Constant">The type of constant.</typeparam>
template <class Constant>
class ConstantBuffer : public Bindable
{
public:
	/// <summary>
	/// Constructs a constant buffer with the default description.
	/// </summary>
	/// <param name="cons">The starting constant.</param>
	/// <param name="slot">The slot to where the constant buffer will be bound to.</param>
	ConstantBuffer(Constant* cons, int slot) : ConstantBuffer(cons, GetDefaultDescription(), slot) {}

	/// <summary>
	/// Creates a constant buffer with a custom description.
	/// </summary>
	/// <param name="cons">The starting constant.</param>
	/// <param name="desc">The description of the constant buffer.</param>
	/// <param name="slot">The slot to where the constant buffer will be bound to.</param>
	ConstantBuffer(Constant* cons, D3D11_BUFFER_DESC desc, int slot) : slot(slot)
	{
		desc.ByteWidth = sizeof(Constant);
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = cons;
		THROW_IF_FAILED(GetDevice()->CreateBuffer(&desc, &data, &constantBuffer));
	}

	virtual void Bind() = 0;

	/// <summary>
	/// Sets new data to the ConstantBuffer.
	/// </summary>
	/// <param name="newConstant">The new data.</param>
	void SetAndUpdate(Constant* newConstant)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		THROW_IF_FAILED(GetContext()->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource));
		memcpy(resource.pData, newConstant, sizeof(Constant));
		GetContext()->Unmap(constantBuffer.Get(), 0);
	}

	/// <summary>
	/// Gets a pointer to the constant buffer.
	/// </summary>
	ID3D11Buffer* Get()
	{
		return constantBuffer.Get();
	}

	int slot;
protected:
	/// <summary>
	/// Gets the default description
	/// </summary>
	D3D11_BUFFER_DESC GetDefaultDescription()
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//	desc.MiscFlags = 0;
		return desc;
	}
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
};


/// <summary>
/// A constant buffer for the vertex shader.
/// </summary>
/// <typeparam name="Constant">The type of constant.</typeparam>
template <class Constant>
class ConstantVS : public ConstantBuffer<Constant>
{
	/* Why this is needed:
	* https://stackoverflow.com/questions/8810224/inheriting-from-a-template-class-in-c
	*/
	using ConstantBuffer<Constant>::slot;
	using ConstantBuffer<Constant>::constantBuffer;
	using Bindable::GetContext;

	using::ConstantBuffer<Constant>::ConstantBuffer;
public:
	void Bind()
	{
		GetContext()->VSSetConstantBuffers(slot, 1, constantBuffer.GetAddressOf());
	}
};

/// <summary>
/// A constant buffer for the pixel shader.
/// </summary>
/// <typeparam name="Constant">The type of constant.</typeparam>
template <class Constant>
class ConstantPS : public ConstantBuffer<Constant>
{
	using ConstantBuffer<Constant>::slot;
	using ConstantBuffer<Constant>::constantBuffer;
	using Bindable::GetContext;

	using::ConstantBuffer<Constant>::ConstantBuffer;
public:
	void Bind()
	{
		GetContext()->PSSetConstantBuffers(slot, 1, constantBuffer.GetAddressOf());
	}
};

/// <summary>
/// A constant buffer for the geo shader.
/// </summary>
/// <typeparam name="Constant">The type of constant.</typeparam>
template <class Constant>
class ConstantGS : public ConstantBuffer<Constant>
{
	using ConstantBuffer<Constant>::slot;
	using ConstantBuffer<Constant>::constantBuffer;
	using Bindable::GetContext;

	using::ConstantBuffer<Constant>::ConstantBuffer;
public:
	void Bind()
	{
		GetContext()->GSSetConstantBuffers(slot, 1, constantBuffer.GetAddressOf());
	}
};
