#include <ECS/Bindable/VertShader.h>
#include <RhyException.h>

#include <d3dcompiler.h>

VertShader::VertShader(LPCWSTR fileName)
{
	/*
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	LPCSTR profile = (GetDevice()->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";

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
	*/
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

ID3D11VertexShader* VertShader::Get()
{
	return shaderPointer.Get();
}
