#include "VertShader.h"
#include "RhyException.h"

#include <d3dcompiler.h>

VertShader::VertShader(LPCWSTR fileName)
{
	THROW_IF_FAILED(D3DReadFileToBlob(fileName, &blob));
	THROW_IF_FAILED(GetDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shaderPointer));
}

void VertShader::Bind()
{
	GetContext()->VSSetShader(shaderPointer.Get(), nullptr, 0);

}

ID3DBlob* VertShader::GetBlob()
{
	return blob.Get();
}
