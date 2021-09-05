#include <ECS/Bindable/Texture.h>
#include <RhyException.h>
#include <RhyWin.h>
#include <RhySTB.h>
#include <2D/TextureGenerator.h>

#include <iostream>
#include <vector>

Texture::Texture(const char* fileName, int slot) : slot(slot), name(fileName)
{
	int bits;

	// load the image
	unsigned char* load = stbi_load(fileName, &width, &height, &bits, 4);

	// throw error if the image could not be loaded.
	if (load == nullptr)
		throw RHY_EXCEP(stbi_failure_reason());

	LoadFromstbi(load);
}

Texture::Texture(TextureGenerator* gen, int slot) : slot(slot), width(gen->GetWidth()), height(gen->GetHeight())
{
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = gen->GetTexture()->data();
	data.SysMemPitch = width * sizeof(unsigned char) * 4;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	THROW_IF_FAILED(GetDevice()->CreateTexture2D(&desc, &data, &tex));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceView = { };
	resourceView.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceView.Texture2D.MipLevels = 1;
	resourceView.Texture2D.MostDetailedMip = 0;
	resourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	THROW_IF_FAILED(GetDevice()->CreateShaderResourceView(tex.Get(), &resourceView, &texturePointer));
}

/*
Texture::Texture(aiTexture* texture, int slot) : slot(slot), width(texture->mWidth), height(texture->mHeight), name(texture->mFilename.C_Str())
{
	if (height == 0)
	{
		int comp = 4;
		unsigned char* load = stbi_load_from_memory(&texture->pcData->r, width, &width, &height, &comp, 4);

		LoadFromstbi(load);
		return;
	}


	// Disable warning about arithmetic overflow
#pragma warning(push)
#pragma warning(disable : 26451)

	std::vector<unsigned char> image(texture->mWidth * texture->mHeight * 4);
	for (int i = 0; i < width * height; i++)
	{
		image[i] = texture->pcData[i].r;
		image[i + 1] = texture->pcData[i].g;
		image[i + 2] = texture->pcData[i].b;
		image[i + 3] = texture->pcData[i].a;
	}

#pragma warning(pop)

	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = image.data();
	data.SysMemPitch = width * sizeof(unsigned char) * 4;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	THROW_IF_FAILED(GetDevice()->CreateTexture2D(&desc, &data, &tex));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceView = { };
	resourceView.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceView.Texture2D.MipLevels = 1;
	resourceView.Texture2D.MostDetailedMip = 0;
	resourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	THROW_IF_FAILED(GetDevice()->CreateShaderResourceView(tex.Get(), &resourceView, &texturePointer));

}
*/

Texture::Texture(Texture* texture, int slot)
	: texturePointer(texture->texturePointer), slot(slot), width(texture->width), height(texture->height), name(texture->name)
{
}

void Texture::Bind()
{
	GetContext()->PSSetShaderResources(slot, 1, texturePointer.GetAddressOf());
}

int Texture::GetWidth()
{
	return width;
}

int Texture::GetHeight()
{
	return height;
}

ID3D11ShaderResourceView* Texture::Get()
{
	return texturePointer.Get();
}

void Texture::LoadFromstbi(unsigned char* load)
{
	// transfer the image to a part of memory that we own.
	std::vector<unsigned char> image(static_cast<size_t>(width) * static_cast<size_t>(height) * 4);
	for (int i = 0; i < width * height * 4; i++)
	{
		image[i] = load[i];
	}

	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = image.data();
	data.SysMemPitch = width * sizeof(unsigned char) * 4;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	THROW_IF_FAILED(GetDevice()->CreateTexture2D(&desc, &data, &tex));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceView = { };
	resourceView.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceView.Texture2D.MipLevels = 1;
	resourceView.Texture2D.MostDetailedMip = 0;
	resourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	THROW_IF_FAILED(GetDevice()->CreateShaderResourceView(tex.Get(), &resourceView, &texturePointer));

	// free the loaded image
	stbi_image_free(load);
}
