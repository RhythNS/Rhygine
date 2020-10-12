#include "PixShader.h"
#include "RhyException.h"

#include <d3dcompiler.h>

PixShader::PixShader(LPCWSTR fileName)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	LPCSTR profile = (GetDevice()->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";

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

	THROW_IF_FAILED(GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shaderPointer));
}

void PixShader::Bind()
{
	GetContext()->PSSetShader(shaderPointer.Get(), nullptr, 0);
}

ID3DBlob* PixShader::GetBlob()
{
	return blob.Get();
}
