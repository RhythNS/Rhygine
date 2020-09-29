#include "Texture.h"
#include "RhyException.h"
#include "RhyWin.h"
#include "RhySTB.h"

#include <iostream>
#include <vector>

Texture::Texture(const char* fileName, int slot) : slot(slot)
{
	int x, y, n;
	unsigned char* image = stbi_load(fileName, &x, &y, &n, 4);

	if (image == nullptr)
		throw RHY_EXCEP("Image could not be loaded");

	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = x;
	desc.Height = y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = &image;
	data.SysMemPitch = x * sizeof(unsigned char) * 4;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	THROW_IF_FAILED(GetDevice()->CreateTexture2D(&desc, &data, &tex));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceView = { };
	resourceView.Format = DXGI_FORMAT_R8G8B8A8_UINT;
	resourceView.Texture2D.MipLevels = 1;
	resourceView.Texture2D.MostDetailedMip = 0;
	resourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	THROW_IF_FAILED(GetDevice()->CreateShaderResourceView(tex.Get(), &resourceView, &texturePointer));

}

void Texture::Bind()
{
	GetContext()->PSSetShaderResources(slot, 1, texturePointer.GetAddressOf());
}
