#include "VertShader.h"
#include "RhyException.h"

#include <d3dcompiler.h>

VertShader::VertShader(LPCWSTR fileName, Microsoft::WRL::ComPtr<ID3DBlob> blob)
{
	THROW_IF_FAILED(D3DReadFileToBlob(fileName, &blob));
	THROW_IF_FAILED(GetDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shaderPointer));
}

void VertShader::Bind()
{
	GetContext()->VSSetShader(shaderPointer.Get(), nullptr, 0);

}
