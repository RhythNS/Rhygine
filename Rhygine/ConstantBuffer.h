#pragma once
#include "Bindable.h"
#include <wrl.h>
#include <vector>

template <class Constant>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(std::vector<Constant> cons);
	ConstantBuffer(std::vector<Constant> cons, D3D11_BUFFER_DESC desc);

	virtual void Bind() = 0;
	void Update();
	std::vector<Constant>& GetConstant();

	int slot;
protected:
	D3D11_BUFFER_DESC GetDefaultDescription();
	std::vector<Constant> constant;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
};


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
