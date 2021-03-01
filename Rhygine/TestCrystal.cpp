#include "TestCrystal.h"
#include "Transform.h"
#include "Drawer.h"
#include "TexLitNormalMapShader.h"
#include "TestLitPlateComponent.h"
#include "SpaceConverter.h"
#include "RotateAround.h"

TestCrystal::TestCrystal(TestLightComponent* tlc) : tlc(tlc)
{ }

void TestCrystal::AddData(GameObject* toAddTo)
{
	Transform* trans = toAddTo->AddComponent<Transform>();
	Drawer* drawer = toAddTo->AddComponent<Drawer>();

	std::vector<VertexPosTangentNormalUV> verts =
	{
		//   x     y     z        tx    ty    tz      nx     ny     nz      u      v
		{ -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f }, // 0 VUL
		{  0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f }, // 1 VUR
		{ -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f }, // 2 VOL
		{  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f }, // 3 VOR

		{  0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f }, // 1 VUR
		{  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f }, // 5 HUR
		{  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f }, // 3 VOR
		{  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f }, // 7 HOR

		{  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f }, // 5 HUR
		{ -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f }, // 4 HUL
		{  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f }, // 7 HOR
		{ -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f }, // 6 HOL

		{ -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f }, // 4 HUL
		{ -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f }, // 0 VUL
		{ -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f }, // 6 HOL
		{ -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f }, // 2 VOL

		{ -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f }, // 4 HUL
		{  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f }, // 5 HUR
		{ -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f }, // 0 VUL
		{  0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f }, // 1 VUR

		{ -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f }, // 2 VOL
		{  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f }, // 3 VOR
		{ -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f }, // 6 HOL
		{  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f }, // 7 HOR
	};

	for (size_t i = 0; i < verts.size(); i += 4)
	{
		RhyM::Vec3 pos1(verts[i].pos.x, verts[i].pos.y, verts[i].pos.z);
		RhyM::Vec3 pos2(verts[i + 1].pos.x, verts[i + 1].pos.y, verts[i + 1].pos.z);
		RhyM::Vec3 pos3(verts[i + 2].pos.x, verts[i + 2].pos.y, verts[i + 2].pos.z);

		RhyM::Vec2 uv1(verts[i].texCoords.u, verts[i].texCoords.v);
		RhyM::Vec2 uv2(verts[i + 1].texCoords.u, verts[i + 1].texCoords.v);
		RhyM::Vec2 uv3(verts[i + 2].texCoords.u, verts[i + 2].texCoords.v);

		RhyM::Vec3 tangent = RhyM::GetTangentFromTextureSpace(pos1, pos2, pos3, uv1, uv2, uv3);

		for (size_t j = 0; j < 4; j++)
		{
			verts[i + j].tangent = { tangent.x, tangent.y, tangent.z };
		}
	}

	std::vector<unsigned short> indexes =
	{
		0,   2,   1,   2,   3,   1,
		4,   6,   5,   6,   7,   5,
		8,   10,  9,   10,  11,  9,
		12,  14,  13,  14,  15,  13,
		16,  18,  17,  18,  19,  17,
		20,  22,  21,  22,  23,  21,
	};

	drawer->AddBindable(std::make_unique<VertBuffer<VertexPosTangentNormalUV>>(verts, 0));
	drawer->AddBindable(std::make_unique<IndexBufferUS>(indexes, 0));
	drawer->AddBindable(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	TexLitNormalMapShader* tlnms = static_cast<TexLitNormalMapShader*>(drawer->AddBindable(std::make_unique<TexLitNormalMapShader>("PresentScene\\crystal\\Crystal_004_basecolor.jpg", "PresentScene\\crystal\\Crystal_004_normal.jpg")));
	tlnms->specStrength = 0.4f;
	tlnms->light = tlc;

	toAddTo->AddComponent<RotateAround>()->rotationSpeed.Set(1.0f, 0.0f, 0.0f);
}
