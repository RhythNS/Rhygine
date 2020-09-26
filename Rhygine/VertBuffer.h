#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>

#include "Bindable.h"

template <class Vertex>
class VertBuffer : public Bindable
{
public:
	VertBuffer(std::vector<Vertex> verts);
	VertBuffer(std::vector<Vertex> verts, D3D11_BUFFER_DESC desc);
	void Bind();
	
	int slot;
protected:
	D3D11_BUFFER_DESC GetDefaultDescription();
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> bufferPointer;
};