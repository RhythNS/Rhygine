#pragma once
#include "Bindable.h"

#include <vector>

/// <summary>
/// Representation of a custom input layout.
/// </summary>
class InputLayout : public Bindable
{
public:
	/// <summary>
	/// Creates the input layout.
	/// </summary>
	/// <param name="desc">The description of the input layout.</param>
	/// <param name="blob">A blob of the shadercode. This can be gotten with shader.GetBlob()</param>
	InputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> desc, ID3DBlob* blob);

	void Bind();

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layoutPointer;
};

