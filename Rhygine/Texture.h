#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(const char* fileName, int slot);
	void Bind();
	int slot;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texturePointer;
};

