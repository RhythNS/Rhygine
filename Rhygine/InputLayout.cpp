#include "InputLayout.h"
#include "RhyException.h"

InputLayout::InputLayout(std::vector< D3D11_INPUT_ELEMENT_DESC> desc, ID3DBlob* blob)
{
	THROW_IF_FAILED( GetDevice()->CreateInputLayout(desc.data(), static_cast<UINT>(desc.size()),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&layoutPointer)
	);
}

void InputLayout::Bind()
{
	GetContext()->IASetInputLayout(layoutPointer.Get());
}

ID3D11InputLayout* InputLayout::Get()
{
	return layoutPointer.Get();
}
