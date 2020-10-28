#include "BulletDebugDraw.h"
#include "RhyBindables.h"
#include "RhyWin.h"
#include "BasicShader.h"
#include "Stage.h"
#include "Camera.h"
#include "Window.h"
#include "Gfx.h"

void BulletDebugDraw::Init(Stage* stage)
{
	this->stage = stage;

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	bindables.push_back(std::make_unique<PixShader>(L"BulletDebugPix.hlsl"));
	bindables.push_back(std::make_unique<VertShader>(L"BulletDebugVert.hlsl"));
	VertShader* vertShader = static_cast<VertShader*>(bindables.back().get());

	ID3DBlob* blob = vertShader->GetBlob();

	bindables.push_back(std::make_unique<InputLayout>(inputLayoutDesc, blob));

	bindables.push_back(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_LINELIST));

	bindables.push_back(std::make_unique< ConstantVS<WorldPos>>(&worldBuffer, 0));
	worldConstant = static_cast<ConstantVS<WorldPos>*>(bindables.back().get());
}

void BulletDebugDraw::setDefaultColors(const DefaultColors& colors)
{
}

void BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	verts.push_back({ 
		from.m_floats[0], 
		from.m_floats[1], 
		from.m_floats[2], 
		(unsigned char)(color.m_floats[0] * 255.0f), 
		(unsigned char)(color.m_floats[1] * 255.0f),
		(unsigned char)(color.m_floats[2] * 255.0f),
		255 
		});
	verts.push_back({ 
		to.m_floats[0], 
		to.m_floats[1], 
		to.m_floats[2], 
		(unsigned char)(color.m_floats[0] * 255.0f),  
		(unsigned char)(color.m_floats[1] * 255.0f),  
		(unsigned char)(color.m_floats[2] * 255.0f),  
		255
		});
}

void BulletDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void BulletDebugDraw::reportErrorWarning(const char* warningString)
{
	OutputDebugString(warningString);
}

void BulletDebugDraw::draw3dText(const btVector3& location, const char* textString)
{

}

void BulletDebugDraw::setDebugMode(int _debugMode)
{
	debugMode = _debugMode;
}

int BulletDebugDraw::getDebugMode() const
{
	return debugMode;
}

void BulletDebugDraw::clearLines()
{
	verts.clear();
}

void BulletDebugDraw::flushLines()
{
	worldBuffer.transform =
		DirectX::XMMatrixTranspose
		(
			*stage->GetCamera()->GetMatrix() *
			DirectX::XMMatrixPerspectiveLH(1.0f, (float)Window::GetInstance()->GetHeight() / (float)Window::GetInstance()->GetWidth(), 0.5f, 100.0f)
		);
	worldConstant->SetAndUpdate(&worldBuffer);

	for (auto& bind : bindables)
		bind->Bind();

	VertBuffer<VertexPosColor> vertexBuffer(verts, 0);
	vertexBuffer.Bind();

	Gfx::GetInstance()->Draw(vertexBuffer.GetSize());
}
