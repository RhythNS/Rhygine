#pragma once
#include "Bindable.h"

/// <summary>
/// Representation of a vertex shader.
/// </summary>
class VertShader : public Bindable
{
public:
	/// <summary>
	/// Standard constructor.
	/// </summary>
	/// <param name="fileName">The path to the vertex shader. Should end with ".hlsl"</param>
	VertShader(LPCWSTR fileName);

	VertShader() = delete;
	
	void Bind();
	
	/// <summary>
	/// Gets the blob to the shader source code.
	/// </summary>
	/// <returns>Dumb pointer to the blob.</returns>
	ID3DBlob* GetBlob();

	/// <summary>
	/// Gets a pointer to the vertex shader.
	/// </summary>
	ID3D11VertexShader* Get();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shaderPointer;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
};
