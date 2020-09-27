#include "GeoShader.h"
#include "RhyException.h"

#include <d3dcompiler.h>

GeoShader::GeoShader(LPCWSTR fileName)
{
	THROW_IF_FAILED(D3DReadFileToBlob(fileName, &blob));
	THROW_IF_FAILED(GetDevice()->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shaderPointer));
}

void GeoShader::Bind()
{
	GetContext()->GSSetShader(shaderPointer.Get(), nullptr, 0);
}

ID3DBlob* GeoShader::GetBlob()
{
	return blob.Get();
}
