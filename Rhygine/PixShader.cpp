#include "PixShader.h"
#include "RhyException.h"

#include <d3dcompiler.h>

PixShader::PixShader(LPCWSTR fileName, Microsoft::WRL::ComPtr<ID3DBlob> blob)
{
	THROW_IF_FAILED(D3DReadFileToBlob(fileName, &blob));
	THROW_IF_FAILED(GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shaderPointer));
}

void PixShader::Bind()
{
	GetContext()->PSSetShader(shaderPointer.Get(), nullptr, 0);
}
