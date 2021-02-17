#include "TestLitPlate.h"
#include "TestLitPlateComponent.h"
#include "TextureChanger.h"
#include "TexLitShader.h"
#include "RotateAround.h"
#include "Verticies.h"

TestLitPlate::TestLitPlate(TestLightComponent* tlc) : tlc(tlc)
{
}

void TestLitPlate::AddData(GameObject* toAddTo)
{
	Transform* transform = toAddTo->AddComponent<Transform>();
	Drawer* drawer = toAddTo->AddComponent<Drawer>();

	transform->localPosition.Set(0.0f, 0.0f, 5.0f);
	//transform->scale.Set(3.0f, 3.0f, 3.0f);

	std::vector<VertexPosNormalUV> verts =
	{
		//   x     y     z        nx     ny     nz      u      v
		{ -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f }, // 0 VUL
		{  0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f }, // 1 VUR
		{ -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f }, // 2 VOL
		{  0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f }, // 3 VOR

		{  0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f }, // 1 VUR
		{  0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f }, // 5 HUR
		{  0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f }, // 3 VOR
		{  0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f }, // 7 HOR

		{  0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f }, // 5 HUR
		{ -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f }, // 4 HUL
		{  0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f }, // 7 HOR
		{ -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f }, // 6 HOL

		{ -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f }, // 4 HUL
		{ -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f }, // 0 VUL
		{ -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f }, // 6 HOL
		{ -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f }, // 2 VOL

		{ -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f }, // 4 HUL
		{  0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f }, // 5 HUR
		{ -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f }, // 0 VUL
		{  0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f }, // 1 VUR

		{ -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f }, // 2 VOL
		{  0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f }, // 3 VOR
		{ -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f }, // 6 HOL
		{  0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f }, // 7 HOR
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

	drawer->AddBindable(std::make_unique<VertBuffer<VertexPosNormalUV>>(verts, 0));
	drawer->AddBindable(std::make_unique<IndexBufferUS>(indexes, 0));
	drawer->AddBindable(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	drawer->AddBindable(std::make_unique<TextureChanger>());
	//drawer->AddBindable(std::make_unique<Texture>("TestModels\\Sprite\\TestImage.png", 0));
	drawer->AddBindable(std::make_unique<Sampler>(0));

	TexLitShader* txs = static_cast<TexLitShader*>(drawer->AddBindable(std::make_unique<TexLitShader>()));
	txs->light = tlc;

	RotateAround* rotateAround = toAddTo->AddComponent<RotateAround>();
	rotateAround->rotationSpeed.Set(0.0f, -1.0f, 0.0f);
	rotateAround->Disable();

	toAddTo->AddComponent<TestLitPlateComponent>();
}
