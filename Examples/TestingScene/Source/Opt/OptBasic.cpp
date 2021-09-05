#include <Opt/OptBasic.h>
#include <ECS/GameObject.h>
#include <ECS/Components/Transform.h>
#include <RhyBindables.h>
#include <ECS/Bindable/Drawer.h>
#include <ECS/Stage.h>
#include <ECS/Bindable/Verticies.h>
#include <ECS/Bindable/BasicShader.h>
#include <Math/Random.h> 

void OptBasic::Init()
{
	tex = std::make_unique<Texture>("PresentScene\\box.png", 0);

	for (int i = 0; i < 10000; i++)
	{
		RhyM::Vec3 randomPos = RhyM::Vec3(RhyM::GetRandomFloat() * 10 - 5, RhyM::GetRandomFloat() * 10 - 5, RhyM::GetRandomFloat() * 10 - 5);
		CreateBox(randomPos);
	}
}

void OptBasic::CreateBox(RhyM::Vec3 pos)
{
	GameObject* go = GetGameObject()->GetStage()->CreateGameObject();

	Transform* trans = go->AddComponent<Transform>();
	trans->localPosition = pos;
	Drawer* drawer = go->AddComponent<Drawer>();

	std::vector<VertexPosUV> verts =
	{
		//   x     y     z        u      v
		{ -0.5f, -0.5f, -0.5f,   0.0f, 1.0f }, // 0 VUL
		{  0.5f, -0.5f, -0.5f,   1.0f, 1.0f }, // 1 VUR
		{ -0.5f,  0.5f, -0.5f,   0.0f, 0.0f }, // 2 VOL
		{  0.5f,  0.5f, -0.5f,   1.0f, 0.0f }, // 3 VOR

		{  0.5f, -0.5f, -0.5f,   0.0f, 1.0f }, // 1 VUR
		{  0.5f, -0.5f,  0.5f,   1.0f, 1.0f }, // 5 HUR
		{  0.5f,  0.5f, -0.5f,   0.0f, 0.0f }, // 3 VOR
		{  0.5f,  0.5f,  0.5f,   1.0f, 0.0f }, // 7 HOR

		{  0.5f, -0.5f,  0.5f,   0.0f, 1.0f }, // 5 HUR
		{ -0.5f, -0.5f,  0.5f,   1.0f, 1.0f }, // 4 HUL
		{  0.5f,  0.5f,  0.5f,   0.0f, 0.0f }, // 7 HOR
		{ -0.5f,  0.5f,  0.5f,   1.0f, 0.0f }, // 6 HOL

		{ -0.5f, -0.5f,  0.5f,  -0.0f, 1.0f }, // 4 HUL
		{ -0.5f, -0.5f, -0.5f,  -1.0f, 1.0f }, // 0 VUL
		{ -0.5f,  0.5f,  0.5f,  -0.0f, 0.0f }, // 6 HOL
		{ -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f }, // 2 VOL

		{ -0.5f, -0.5f,  0.5f,   0.0f, 1.0f }, // 4 HUL
		{  0.5f, -0.5f,  0.5f,   1.0f, 1.0f }, // 5 HUR
		{ -0.5f, -0.5f, -0.5f,   0.0f, 0.0f }, // 0 VUL
		{  0.5f, -0.5f, -0.5f,   1.0f, 0.0f }, // 1 VUR

		{ -0.5f,  0.5f, -0.5f,   0.0f, 1.0f }, // 2 VOL
		{  0.5f,  0.5f, -0.5f,   1.0f, 1.0f }, // 3 VOR
		{ -0.5f,  0.5f,  0.5f,   0.0f, 0.0f }, // 6 HOL
		{  0.5f,  0.5f,  0.5f,   1.0f, 0.0f }, // 7 HOR
	};

	std::vector<unsigned short> indexes =
	{
		0,   2,   1,   2,   3,   1,
		4,   6,   5,   6,   7,   5,
		8,   10,  9,   10,  11,  9,
		12,  14,  13,  14,  15,  13,
		16,  18,  17,  18,  19,  17,
		20,  22,  21,  22,  23,  21,
	};

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "texCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	drawer->AddBindable(std::make_unique<BasicShader>(L"TexPix.hlsl", L"TexVert.hlsl", &inputLayoutDesc));
	drawer->AddBindable(std::make_unique<VertBuffer<VertexPosUV>>(verts, 0));
	drawer->AddBindable(std::make_unique<IndexBufferUS>(indexes, 0));
	drawer->AddBindable(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	drawer->AddBindable(std::make_unique<Sampler>(0));
	drawer->AddBindable(std::make_unique<Texture>(tex.get(), 0));
}
