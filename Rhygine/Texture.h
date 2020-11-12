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

	/// <summary>
	/// Gets the width of the texture.
	/// </summary>
	int GetWidth();
	/// <summary>
	/// Gets the height of the texture.
	/// </summary>
	int GetHeight();

	/// <summary>
	/// Gets a pointer to the texture resource.
	/// </summary>
	ID3D11ShaderResourceView* Get();

	int slot;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texturePointer;
	int width;
	int height;
};
