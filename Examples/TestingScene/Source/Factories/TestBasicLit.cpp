#include <Factories/TestBasicLit.h>
#include <ECS/Bindable/Verticies.h>
#include <Components/RotateAround.h>
#include <CC/Shaders/BasicTextureLit.h>
#include <ECS/Bindable/Texture.h>

TestBasicLit::TestBasicLit(std::string imagePath, TestLightComponent* tlc) : imagePath(imagePath), tlc(tlc)
{
}

TestBasicLit::TestBasicLit(Texture* texture, TestLightComponent* tlc) : texture(texture), tlc(tlc)
{
}

void TestBasicLit::AddData(GameObject* toAddTo)
{
	Transform* trans = toAddTo->AddComponent<Transform>();
	Drawer* drawer = toAddTo->AddComponent<Drawer>();

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
	drawer->AddBindable(std::make_unique<Sampler>(0));
	BasicTextureLit* btl;
	if (!texture)
		btl = static_cast<BasicTextureLit*>(drawer->AddBindable(std::make_unique<BasicTextureLit>(imagePath.c_str())));
	else
		btl = static_cast<BasicTextureLit*>(drawer->AddBindable(std::make_unique<BasicTextureLit>(texture)));

	btl->light = tlc;

	RotateAround* rotateAround = toAddTo->AddComponent<RotateAround>();
	rotateAround->rotationSpeed.Set(-1.0f, 0.0f, 0.0f);
	rotateAround->Disable();


}
