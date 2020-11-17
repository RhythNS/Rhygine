#include "TestFloor.h"
#include "Transform.h"
#include "Drawer.h"
#include "Verticies.h"
#include "TexLitNormalMapShader.h"
#include "RotateAround.h"
#include "TestLitPlateComponent.h"

#include "RhyMath.h"

TestFloor::TestFloor(float width, float depth, TestLightComponent* tlc) : width(width), depth(depth), tlc(tlc)
{ }

void TestFloor::AddData(GameObject* toAddTo)
{
	Transform* trans = toAddTo->AddComponent<Transform>();
	Drawer* drawer = toAddTo->AddComponent<Drawer>();

	RhyM::Vec3 pos1(-width / 2, 0.0f, depth / 2); //3
	RhyM::Vec3 pos2(-width / 2, 0.0f, -depth / 2); //0
	RhyM::Vec3 pos3(width / 2, 0.0f, -depth / 2); //1
	RhyM::Vec3 pos4(width / 2, 0.0f, depth / 2); //4

	RhyM::Vec2 uv1(-depth / 2, width / 2);
	RhyM::Vec2 uv2(-depth / 2, -width / 2);
	RhyM::Vec2 uv3(depth / 2, -width / 2);
	RhyM::Vec2 uv4(depth / 2, width / 2);

	RhyM::Vec3 edge1 = pos2 - pos1;
	RhyM::Vec3 edge2 = pos3 - pos1;
	RhyM::Vec2 deltaUV1 = uv2 - uv1;
	RhyM::Vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	RhyM::Vec3 tangent, normal;

	tangent.m_floats[0] = f * (deltaUV2.y * edge1.m_floats[0] - deltaUV1.y * edge2.m_floats[0]);
	tangent.m_floats[1] = f * (deltaUV2.y * edge1.m_floats[1] - deltaUV1.y * edge2.m_floats[1]);
	tangent.m_floats[2] = f * (deltaUV2.y * edge1.m_floats[2] - deltaUV1.y * edge2.m_floats[2]);

	normal.setValue(0.0f, 1.0f, 0.0f);

	tangent.normalize();
	struct Vertex {
		struct {
			float x, y, z;
		} pos;
		struct {
			float x, y, z;
		} tangent;
		struct {
			float x, y, z;
		} normal;
		struct {
			float u, v;
		} texCoords;
	};

	std::vector<Vertex> verts =
	{
		//     x        y         z       																				    u         v
		{ -width / 2,  0.0f, -depth / 2, tangent[0], tangent[1], tangent[2], normal[0], normal[1], normal[2], -depth / 2,  width / 2 },
		{  width / 2,  0.0f, -depth / 2, tangent[0], tangent[1], tangent[2], normal[0], normal[1], normal[2],  depth / 2,  width / 2 },
		{ -width / 2,  0.0f,  depth / 2, tangent[0], tangent[1], tangent[2], normal[0], normal[1], normal[2], -depth / 2, -width / 2 },
		{  width / 2,  0.0f,  depth / 2, tangent[0], tangent[1], tangent[2], normal[0], normal[1], normal[2],  depth / 2, -width / 2 }
	};

	std::vector<unsigned short> indexes =
	{
		0,   2,   1,   2,   3,   1,
	};

	drawer->AddBindable(std::make_unique<VertBuffer<Vertex>>(verts, 0));
	drawer->AddBindable(std::make_unique<IndexBufferUS>(indexes, 0));
	drawer->AddBindable(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	TexLitNormalMapShader* tlnms = static_cast<TexLitNormalMapShader*>(drawer->AddBindable(std::make_unique<TexLitNormalMapShader>("PresentScene\\wood\\Wood_planks_011_basecolor.jpg", "PresentScene\\wood\\Wood_planks_011_normal.jpg", desc)));
	tlnms->specStrength = 20.0f;
	tlnms->light = tlc;

	toAddTo->AddComponent<TestLitPlateComponent>();
}
