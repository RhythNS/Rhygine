#pragma once
#include "Bindable.h"

class TextureGenerator;
struct aiTexture;

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

	/// <summary>
	/// Creates a texture from a texture generator.
	/// </summary>
	/// <param name="gen">The texture generator where the pixels are read from.</param>
	/// <param name="slot">The slot to where the texture will be bound to.</param>
	Texture(TextureGenerator* gen, int slot);

	/// <summary>
	/// Creates a texture from assimp.
	/// </summary>
	/// <param name="texture">A reference to the image.</param>
	/// <param name="slot">The slot to where the texture will be bound to.</param>
	Texture(aiTexture* texture, int slot);

	/// <summary>
	/// Creates a copy of a texture.
	/// </summary>
	/// <param name="texturePointer">The texture to copy.</param>
	/// <param name="slot">The slot to where the texture will be bound to.</param>
	Texture(Texture* texture, int slot);

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
	void LoadFromstbi(unsigned char* load);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texturePointer;
	int width;
	int height;
};
