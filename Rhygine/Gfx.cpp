#include "Gfx.h"
#include "RhyException.h"
#include "Window.h"
#include "Scene.h"
#include "Keys.h"

#include <Windows.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

Gfx::Gfx(Window* window) : window(window), camera({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f })
{
	HWND windowHandle = *window->GetWindowHandle();

	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = windowHandle;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;

	IDXGIAdapter* pAdapter = nullptr;
	D3D_DRIVER_TYPE	DriverType = D3D_DRIVER_TYPE_HARDWARE;
	HMODULE	Software = nullptr;
	UINT flags = 0;
#ifndef DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	const D3D_FEATURE_LEVEL* pFeatureLevels = nullptr;
	UINT FeatureLevels = 0;
	UINT SDKVersion = D3D11_SDK_VERSION;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;

	THROW_IF_FAILED(D3D11CreateDeviceAndSwapChain(
		pAdapter,
		DriverType,
		Software,
		flags,
		pFeatureLevels,
		FeatureLevels,
		SDKVersion,
		&desc,
		&swap,
		&device,
		pFeatureLevel,
		&context
	));

	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	THROW_IF_FAILED(swap->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	THROW_IF_FAILED(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &target));

	D3D11_DEPTH_STENCIL_DESC stencilDesc = { 0 };
	stencilDesc.DepthEnable = true;
	stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> stencilState;
	THROW_IF_FAILED(device->CreateDepthStencilState(&stencilDesc, &stencilState));

	context->OMSetDepthStencilState(stencilState.Get(), 1);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTex;
	D3D11_TEXTURE2D_DESC depthTexDesc = { 0 };
	depthTexDesc.Width = window->GetWidth();
	depthTexDesc.Height = window->GetHeight();
	depthTexDesc.MipLevels = 1;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTexDesc.SampleDesc.Count = 1;
	depthTexDesc.SampleDesc.Quality = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	THROW_IF_FAILED(device->CreateTexture2D(&depthTexDesc, nullptr, &depthStencilTex));

	D3D11_DEPTH_STENCIL_VIEW_DESC stencilViewDesc = { };
	stencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	stencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencilViewDesc.Texture2D.MipSlice = 0;
	THROW_IF_FAILED(device->CreateDepthStencilView(depthStencilTex.Get(), &stencilViewDesc, &depthStencilView));

	context->OMSetRenderTargets(1, target.GetAddressOf(), depthStencilView.Get());
}

void Gfx::BeginDraw()
{
	context->ClearRenderTargetView(target.Get(), window->GetCurrentScene()->GetClearColor());
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Gfx::DebugDraw()
{
	struct Vertex {
		struct {
			float x, y, z;
		} pos;
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};


	Vertex verts[] = {
		{-0.5f, -0.5f, 0.5f, 255,0,0,0},
		{0.5f, -0.5f, 0.5f, 0,255,0,0},
		{0.5f, -0.5f, -0.5f, 0,0,255,0},
		{-0.5f, -0.5f, -0.5f, 255,0,0},
		{0.0f, 0.5f, 0.0f, 0,255,0,0},
	};

	const unsigned short indexes[]{
		3,4,2, 2,4,1,
		1,4,0, 0,4,3,
		2,1,0, 0,3,2
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertBuffer;
	D3D11_BUFFER_DESC vertBufferDesc = { };
	vertBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertBufferDesc.CPUAccessFlags = 0;
	vertBufferDesc.MiscFlags = 0;
	vertBufferDesc.ByteWidth = sizeof(verts);
	vertBufferDesc.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA vertData = {  };
	vertData.pSysMem = verts;
	THROW_IF_FAILED(device->CreateBuffer(&vertBufferDesc, &vertData, &vertBuffer));

	unsigned int strides = sizeof(Vertex);
	unsigned int vertOffset = 0;
	context->IASetVertexBuffers(0, 1, vertBuffer.GetAddressOf(), &strides, &vertOffset);

	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	D3D11_BUFFER_DESC indexBufferDesc = {  };
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.ByteWidth = sizeof(indexes);
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA indexData = { };
	indexData.pSysMem = indexes;
	THROW_IF_FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer));

	DXGI_FORMAT format = DXGI_FORMAT_R16_UINT;
	unsigned int indexOffset = 0;
	context->IASetIndexBuffer(indexBuffer.Get(), format, indexOffset);

	float angle = window->time.GetTimeSinceStart(), x = 0, y = 0, z = 0;
	//float angle = 0, x = 0, z = 0;

	camera.HandleInput(window);

	struct MatrixConstantBuffer {
		DirectX::XMMATRIX transform;
	};
	MatrixConstantBuffer matConsBuff{
		{
			DirectX::XMMatrixTranspose(
					DirectX::XMMatrixScaling(4.0f, 4.0f, 4.0f) *
					DirectX::XMMatrixRotationRollPitchYaw(0, angle, 0) *
					DirectX::XMMatrixTranslation(x, y, z) *
					camera.GetMatrix() *
					DirectX::XMMatrixPerspectiveLH(1.0f, (float)window->GetHeight() / (float)window->GetWidth(), 0.5f, 100.0f)
				)
			}
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> transformConstantBuffer;
	D3D11_BUFFER_DESC constantBufferDesc = { 0 };
	constantBufferDesc.ByteWidth = sizeof(matConsBuff);
	constantBufferDesc.StructureByteStride = 0;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA transformConstantData = { 0 };
	transformConstantData.pSysMem = &matConsBuff;
	THROW_IF_FAILED(device->CreateBuffer(&constantBufferDesc, &transformConstantData, &transformConstantBuffer));

	context->VSSetConstantBuffers(0, 1, transformConstantBuffer.GetAddressOf());

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	THROW_IF_FAILED(D3DReadFileToBlob(L"BasicPix.cso", &blob));
	THROW_IF_FAILED(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));

	context->PSSetShader(pixelShader.Get(), nullptr, 0);

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertShader;
	THROW_IF_FAILED(D3DReadFileToBlob(L"BasicVert.cso", &blob));
	THROW_IF_FAILED(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertShader));

	context->VSSetShader(vertShader.Get(), nullptr, 0);

	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	THROW_IF_FAILED(
		device->CreateInputLayout(
			inputLayoutDesc,
			std::size(inputLayoutDesc),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&inputLayout)
	);

	context->IASetInputLayout(inputLayout.Get());

	context->OMSetRenderTargets(1, target.GetAddressOf(), nullptr);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT viewport = { 0 };
	viewport.Width = window->GetWidth();
	viewport.Height = window->GetHeight();
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1, &viewport);

	//context->DrawIndexed(std::size(indexes), 0, 0);
	context->DrawIndexed((UINT)std::size(indexes), 0, 0);
}

void Gfx::EndDraw()
{
	THROW_IF_FAILED(swap->Present(1, 0));
}
