#pragma once
#include "Bindable.h"

/// <summary>
/// Representation of a pixel shader.
/// </summary>
class PixShader : public Bindable
{
public:
	/// <summary>
	/// Standard constructor.
	/// </summary>
	/// <param name="fileName">The path to the pixel shader source code. Should end with ".hlsl"</param>
	PixShader(LPCWSTR fileName);
	void Bind();
	/// <summary>
	/// Gets the blob to the shader source code.
	/// </summary>
	/// <returns>Dumb pointer to the blob.</returns>
	ID3DBlob* GetBlob();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shaderPointer;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
};

