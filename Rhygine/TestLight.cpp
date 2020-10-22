#include "TestLight.h"
#include "RhyBindables.h"
#include "Rhyimgui.h"
#include "Window.h"
#include "RhyWin.h"
#include "TestLightComponent.h"
#include "Transform.h"
#include "Drawer.h"
#include "BasicShader.h"
#include "ShapeFactory.h"

#include <cmath>

void TestLight::AddData(GameObject* toAddTo)
{
	Transform* transform = AddTransform(toAddTo);
	transform->scale.Set(0.1f, 0.1f, 0.1f);
	Drawer* drawer = AddDrawer(toAddTo);

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

	VertIndexes<VertexPosColor, unsigned short> vertIndexes = ShapeFactory::GetCubePosColor<unsigned short>();
	for (int i = 0; i < vertIndexes.verticies.size(); i++)
	{
		vertIndexes.verticies[i].color.r = 255;
	}

	drawer->AddBindable(std::make_unique<VertBuffer<VertexPosColor>>(vertIndexes.verticies, 0));
	drawer->AddBindable(std::make_unique<IndexBufferUS>(vertIndexes.indicies, 0));
	drawer->AddBindable(std::make_unique<PrimitiveTopolpgy>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	drawer->AddBindable(std::make_unique<BasicShader>(L"BasicPix.hlsl", L"BasicVert.hlsl", &inputLayoutDesc));

	toAddTo->AddComponent<TestLightComponent>();

	transform->position.z = -2.0f;
}
