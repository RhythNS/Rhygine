#include "ConstantBuffer.h"
#include "RhyException.h"

template<class Constant>
ConstantBuffer<Constant>::ConstantBuffer(std::vector<Constant> cons) : ConstantBuffer(cons, GetDefaultDescription())
{
}

template<class Constant>
ConstantBuffer<Constant>::ConstantBuffer(std::vector<Constant> cons, D3D11_BUFFER_DESC desc) : constant(cons)
{
	desc.ByteWidth = sizeof(Constant) * cons.size();
	desc.StructureByteStride = sizeof(Constant);

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = &cons.data();
	THROW_IF_FAILED(GetDevice()->CreateBuffer(&desc, &data, &constantBuffer));
}

template<class Constant>
void ConstantBuffer<Constant>::Update()
{
	GetContext()->UpdateSubresource(constantBuffer.Get(), 0, 0, &constant, 0, 0);
}

template<class Constant>
std::vector<Constant>& ConstantBuffer<Constant>::GetConstant()
{
	return &constant;
}

template<class Constant>
D3D11_BUFFER_DESC ConstantBuffer<Constant>::GetDefaultDescription()
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	return desc;
}
