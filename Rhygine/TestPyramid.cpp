#include "TestPyramid.h"
#include "VertBuffer.h"
#include "PixShader.h"
#include "VertShader.h"
#include "InputLayout.h"
#include "PrimitiveTopolpgy.h"
#include "Window.h"
#include "Gfx.h"
#include "TestCamera.h"

#include <vector>

TestPyramid::TestPyramid() : consBuffer(nullptr), indexBuffer(nullptr)
{
}

void TestPyramid::Init()
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

	std::vector<Vertex> verts = {
		{-0.5f, -0.5f, 0.5f, 255,0,0,0},
		{0.5f, -0.5f, 0.5f, 0,255,0,0},
		{0.5f, -0.5f, -0.5f, 0,0,255,0},
		{-0.5f, -0.5f, -0.5f, 255,0,0},
		{0.0f, 0.5f, 0.0f, 0,255,0,0},
	};

	std::vector<unsigned short> indexes{
		3,4,2, 2,4,1,
		1,4,0, 0,4,3,
		2,1,0, 0,3,2
	};

	bindables.push_back(std::make_unique<VertBuffer<Vertex>>(verts, 0));

	bindables.push_back(std::make_unique<IndexBufferUS>(indexes));
	indexBuffer = static_cast<IndexBufferUS*>(bindables.end()->get());

	std::vector<MatrixConstantBuffer> matConsBuffer;
	matConsBuffer.push_back(
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixScaling(4.0f, 4.0f, 4.0f) *
				DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0) *
				DirectX::XMMatrixTranslation(0, 0, -10) *
				Window::GetInstance()->GetGfx()->camera.GetMatrix() *
				DirectX::XMMatrixPerspectiveLH(1.0f, (float)Window::GetInstance()->GetHeight() / (float)Window::GetInstance()->GetWidth(), 0.5f, 100.0f)
			)
		}
	);

	bindables.push_back(std::make_unique<ConstantVS<MatrixConstantBuffer>>(matConsBuffer));
	consBuffer = static_cast<ConstantVS<MatrixConstantBuffer>*>(bindables.end()->get());

	bindables.push_back(std::make_unique<PixShader>(L"BasicPix.cso"));
	bindables.push_back(std::make_unique<VertShader>(L"BasicVert.cso"));
	ID3DBlob* blob = ( static_cast<VertShader*>( bindables.end()->get() ) )->GetBlob();

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	bindables.push_back(std::make_unique<InputLayout>(inputLayoutDesc, blob));

	bindables.push_back(std::make_unique<PrimitiveTopolpgy>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}

void TestPyramid::Update()
{
	consBuffer->GetConstant()[0].transform = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(4.0f, 4.0f, 4.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0) *
		DirectX::XMMatrixTranslation(0, 0, -10) *
		Window::GetInstance()->GetGfx()->camera.GetMatrix() *
		DirectX::XMMatrixPerspectiveLH(1.0f, (float)Window::GetInstance()->GetHeight() / (float)Window::GetInstance()->GetWidth(), 0.5f, 100.0f)
	);
	consBuffer->Update();
}

void TestPyramid::Draw()
{
	for (auto& bindable : bindables)
		bindable->Bind();

	Window::GetInstance()->GetGfx()->DrawIndexed(indexBuffer->GetSize());
}
