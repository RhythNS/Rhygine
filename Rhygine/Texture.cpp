#include "Texture.h"
#include "RhyException.h"
#include "RhyWin.h"
#include "RhySTB.h"

#include <iostream>
#include <vector>

Texture::Texture(const char* fileName, int slot) : slot(slot)
{
	int x, y, n;

	unsigned char* load = stbi_load(fileName, &x, &y, &n, 4);

	if (load == nullptr)
		throw RHY_EXCEP(stbi_failure_reason());

	std::vector<unsigned char> image(x * y * 4);

	for (int i = 0; i < x * y * 4; i++)
	{
		image[i] = load[i];
	}

	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = x;
	desc.Height = y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = image.data();
	data.SysMemPitch = x * sizeof(unsigned char) * 4;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	THROW_IF_FAILED(GetDevice()->CreateTexture2D(&desc, &data, &tex));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceView = { };
	resourceView.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceView.Texture2D.MipLevels = 1;
	resourceView.Texture2D.MostDetailedMip = 0;
	resourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	THROW_IF_FAILED(GetDevice()->CreateShaderResourceView(tex.Get(), &resourceView, &texturePointer));

	stbi_image_free(load);
}

void Texture::Bind()
{
	GetContext()->PSSetShaderResources(slot, 1, texturePointer.GetAddressOf());
}
