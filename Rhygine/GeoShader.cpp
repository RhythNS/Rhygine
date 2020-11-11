#include "GeoShader.h"
#include "RhyException.h"

#include <d3dcompiler.h>

GeoShader::GeoShader(LPCWSTR fileName)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	LPCSTR profile = (GetDevice()->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "gs_5_0" : "gs_4_0";

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	HRESULT hr;
	if (
		FAILED(hr = D3DCompileFromFile(fileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", profile, flags, 0, &blob, &errorBlob))
		)
	{
		if (errorBlob)
		{
			OutputDebugString((char*)errorBlob->GetBufferPointer());
			throw RHY_EXCEP((char*)errorBlob->GetBufferPointer());
		}

		THROW_IF_FAILED(hr);
	}

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

ID3D11GeometryShader* GeoShader::Get()
{
	return shaderPointer.Get();
}
