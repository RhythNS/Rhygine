#include <Components/BulletShowcaser.h>
#include <ECS/GameObject.h>
#include <ECS/Stage.h>
#include <ECS/Components/Transform.h>
#include <ECS/Bindable/Drawer.h>
#include <ECS/Bindable/BasicShader.h>
#include <Math/Random.h>
#include <CC/DeleteWhenBelow.h>
#include <Input/Keys.h>
#include <Bullet/BulletConverter.h>
#include <RhyBullet.h>


#include <algorithm>

void BulletShowcaser::Init()
{
	boxTexture = std::make_unique<Texture>("Assets\\PresentScene\\box.png", 0);

	RhyBullet::RigidBody* ground = CreateBox(baseSpawnPoint - RhyM::Vec3(0.0f, 3.0f, 0.0f), RhyM::Vec3(5.0f, 1.0f, 5.0f), 0.0f);
}

void BulletShowcaser::Update()
{
	timer -= GetDelta();

	if (GetKeys()->IsKeyDownThisFrame('G'))
		timer = 0;

	if (timer > 0)
		return;

	timer = secondsPerBox;

	RhyM::Vec3 position =
		RhyM::Vec3(
			RhyM::GetRandomFloat() * 5.0f - 2.5f,
			20.0f,
			RhyM::GetRandomFloat() * 5.0f - 2.5f
		);

	RhyBullet::RigidBody* box = CreateBox(baseSpawnPoint + position, RhyM::Vec3(1.0f, 1.0f, 1.0f), 1.0f);
	box->GetGameObject()->AddComponent<DeleteWhenBelow>();
}

RhyBullet::RigidBody* BulletShowcaser::CreateBox(RhyM::Vec3 position, RhyM::Vec3 size, float mass)
{
	GameObject* go = GetGameObject()->GetStage()->CreateGameObject();

	Transform* trans = go->AddComponent<Transform>();
	trans->localPosition = position;
	trans->localScale = size;
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
	drawer->AddBindable(std::make_unique<BasicShader>(L"Assets\\Shader\\TexPix.cso", L"Assets\\Shader\\TexVert.cso", &inputLayoutDesc));
	drawer->AddBindable(std::make_unique<VertBuffer<VertexPosUV>>(verts, 0));
	drawer->AddBindable(std::make_unique<IndexBufferUS>(indexes, 0));
	drawer->AddBindable(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	drawer->AddBindable(std::make_unique<Sampler>(0));
	drawer->AddBindable(std::make_unique<Texture>(boxTexture.get(), 0));

	RhyBullet::RigidBody* body = go->AddComponent<RhyBullet::RigidBody>();
	body->Create(mass, std::make_unique<btBoxShape>(RhyBullet::Vec3ToBullet(size / 2)), size / 2);

	return body;
}
