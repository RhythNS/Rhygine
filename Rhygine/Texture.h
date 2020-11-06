#pragma once
#include "Bindable.h"

/// <summary>
/// Representation of a texture.
/// </summary>
class Texture : public Bindable
{
public:
	/// <summary>
	/// Creates a texture.
	/// </summary>
	/// <param name="fileName">The path to the texture.</param>
	/// <param name="slot">The slot to where the texture will be bound to.</param>
	Texture(const char* fileName, int slot);

	void Bind();
	
	int slot;

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texturePointer;
};

