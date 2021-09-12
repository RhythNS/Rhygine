#include <React/ReactDebugRenderer.h>
#include <ECS/Stage.h>
#include <ECS/GameObject.h>
#include <ECS/Components/Transform.h>
#include <ECS/Components/Camera.h>
#include <Core/Window.h>
#include <Core/Gfx.h>

#include <reactphysics3d/utils/DebugRenderer.h>
#include <reactphysics3d/containers/List.h>

#include <RhyBindables.h>
#include <RhyWin.h>

void RhyReact::ReactDebugRenderer::Init(Stage* stage)
{
	this->stage = stage;

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	bindables.push_back(std::make_unique<PixShader>(L"Assets\\Shader\\ReactDebugPix.cso"));
	bindables.push_back(std::make_unique<VertShader>(L"Assets\\Shader\\ReactDebugVert.cso"));
	VertShader* vertShader = static_cast<VertShader*>(bindables.back().get());

	ID3DBlob* blob = vertShader->GetBlob();

	bindables.push_back(std::make_unique<InputLayout>(inputLayoutDesc, blob));

	bindables.push_back(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_LINELIST));

	bindables.push_back(std::make_unique< ConstantVS<WorldPos>>(&worldBuffer, 0));
	worldConstant = static_cast<ConstantVS<WorldPos>*>(bindables.back().get());
}

void RhyReact::ReactDebugRenderer::Draw(reactphysics3d::DebugRenderer& debugRenderer)
{
	verts.clear();

	reactphysics3d::List<reactphysics3d::DebugRenderer::DebugLine> lines = debugRenderer.getLines();

	reactphysics3d::List<reactphysics3d::DebugRenderer::DebugTriangle> triangles = debugRenderer.getTriangles();

	if (lines.size() == 0 && triangles.size() == 0)
		return;

	for (size_t i = 0; i < lines.size(); i++)
	{
		PushVertex(lines[i].point1, lines[i].color1);
		PushVertex(lines[i].point2, lines[i].color2);
	}

	for (size_t i = 0; i < triangles.size(); i++)
	{
		PushVertex(triangles[i].point1, triangles[i].color1);
		PushVertex(triangles[i].point2, triangles[i].color2);
		PushVertex(triangles[i].point3, triangles[i].color3);
	}

	// Get the camera matrix and give it to the shader
	worldBuffer.transform =
		DirectX::XMMatrixTranspose
		(
			*stage->Get3DCamera()->GetMatrix() *
			DirectX::XMMatrixPerspectiveLH(
				1.0f,
				(float)Window::GetInstance()->GetHeight() / (float)Window::GetInstance()->GetWidth(),
				0.5f,
				100.0f)
		);
	worldConstant->SetAndUpdate(&worldBuffer);

	// bind all bindables
	for (auto& bind : bindables)
		bind->Bind();

	// make a new vertexbuffer with the verticies that were given during the draw process
	VertBuffer<VertexPosRhyColor> vertexBuffer(verts, 0);
	vertexBuffer.Bind();

	// Make the draw call to Gfx.
	Gfx::GetInstance()->Draw(vertexBuffer.GetSize());
}

void RhyReact::ReactDebugRenderer::PushVertex(const reactphysics3d::Vector3& pos, const unsigned int& color)
{
	verts.push_back(
		{
			pos.x,
			pos.y,
			pos.z,
			static_cast<RhyC::color>(color)
		}
	);
}
