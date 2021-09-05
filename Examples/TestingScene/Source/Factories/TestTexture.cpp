#include <Factories/TestTexture.h>
#include <Core/Window.h>
#include <Bindables/TextureChanger.h>
#include <ECS/Bindable/BasicShader.h>
#include <RhyBindables.h>

#include <string>

void TestTexture::AddData(GameObject* toAddTo)
{
	Transform* transform = AddTransform(toAddTo);
	Drawer* drawer = AddDrawer(toAddTo);

	transform->localPosition = { 10.0f, 0.0f, 10.0f };

	struct Vertex {
		struct {
			float x, y, z;
		} pos;
		struct {
			float u;
			float v;
		} texCoords;
	};

	std::vector<Vertex> verts =
	{
		{-1.0f, -1.0f,  0.0f, 0.0f, 1.0f},
		{-1.0f,  1.0f,  0.0f, 0.0f, 0.0f},
		{ 1.0f,  1.0f,  0.0f, 1.0f, 0.0f},
		{ 1.0f, -1.0f,  0.0f, 1.0f, 1.0f}
	};

	std::vector<unsigned short> indexes =
	{
		0, 1, 2,
		3, 0, 2
	};

	drawer->AddBindable(std::make_unique<VertBuffer<Vertex>>(verts, 0));

	drawer->AddBindable(std::make_unique<IndexBufferUS>(indexes, 0));

	drawer->AddBindable(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	drawer->AddBindable(std::make_unique<Sampler>(0));

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "texCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	drawer->AddBindable(std::make_unique<BasicShader>(L"Assets\\Shader\\TexPix.cso", L"Assets\\Shader\\TexVert.cso", &inputLayoutDesc));

	drawer->AddBindable(std::make_unique<TextureChanger>());
}
