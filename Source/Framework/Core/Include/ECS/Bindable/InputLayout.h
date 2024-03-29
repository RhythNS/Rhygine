#pragma once
#include <ECS/Bindable/Bindable.h>

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

	/// <summary>
	/// Gets a pointer to the input layout.
	/// </summary>
	ID3D11InputLayout* Get();

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layoutPointer;
};

