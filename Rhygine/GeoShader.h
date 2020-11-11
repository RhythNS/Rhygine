#pragma once
#include "Bindable.h"

/// <summary>
/// Representation of a GeoShader.
/// </summary>
class GeoShader : public Bindable
{
public:
	/// <summary>
	/// Standard constructor.
	/// </summary>
	/// <param name="fileName">The path to the GeoShader source code. Should end with ".hlsl"</param>
	GeoShader(LPCWSTR fileName);
	GeoShader() = delete;
	void Bind();
	/// <summary>
	/// Gets the blob to the shader source code.
	/// </summary>
	/// <returns>Dumb pointer to the blob.</returns>
	ID3DBlob* GetBlob();

	/// <summary>
	/// Gets a pointer to the geo shader.
	/// </summary>
	ID3D11GeometryShader* Get();
private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> shaderPointer;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
};

