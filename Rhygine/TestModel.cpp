#include "TestModel.h"
#include "RhyException.h"
#include "VertBuffer.h"
#include "PixShader.h"
#include "VertShader.h"
#include "InputLayout.h"
#include "PrimitiveTopolpgy.h"
#include "Sampler.h"
#include "Texture.h"
#include "RhyWin.h"
#include "Window.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void TestModel::Init()
{
	transform.position.Set(-10.0f, 0.0f, 10.0f);

	struct Vertex {
		struct {
			float x, y, z;
		} pos;
		struct {
			float u;
			float v;
		} texCoords;
	};

	std::vector<Vertex> verts;
	std::vector<unsigned short> indexes;

	const std::string pFile = "TestModels\\spot.obj";
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices);

	const aiMesh* mesh = scene->mMeshes[0];

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D* texVec = &mesh->mTextureCoords[0][i];
		verts.push_back(
			{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, texVec->x, 1-texVec->y}
		);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		indexes.push_back(face.mIndices[0]);
		indexes.push_back(face.mIndices[1]);
		indexes.push_back(face.mIndices[2]);
	}

	bindables.push_back(std::make_unique<VertBuffer<Vertex>>(verts, 0));

	bindables.push_back(std::make_unique<IndexBufferUS>(indexes, 0));
	indexBuffer = static_cast<IndexBufferUS*>(bindables[bindables.size() - 1].get());

	bindables.push_back(std::make_unique<PrimitiveTopolpgy>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	CreateTransform();

	bindables.push_back(std::make_unique<Texture>("TestModels\\spot_texture.png", 0));

	bindables.push_back(std::make_unique<Sampler>(0));

	bindables.push_back(std::make_unique<PixShader>(L"TexPix.cso"));
	bindables.push_back(std::make_unique<VertShader>(L"TexVert.cso"));
	ID3DBlob* blob = (static_cast<VertShader*>(bindables[bindables.size() - 1].get()))->GetBlob();

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "texCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	bindables.push_back(std::make_unique<InputLayout>(inputLayoutDesc, blob));
}

void TestModel::Update()
{
	float delta = Window::GetInstance()->time.GetDelta();
	transform.rotation = transform.rotation * Quat(delta * 0.5f, delta * 0.8f, delta);
	Gameobject::Update();
}
