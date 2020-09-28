#include "TestModel.h"
#include "RhyException.h"
#include "VertBuffer.h"
#include "PixShader.h"
#include "VertShader.h"
#include "InputLayout.h"
#include "PrimitiveTopolpgy.h"
#include "RhyWin.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

void TestModel::Init()
{
	position = { -10.0f, 0.0f, 10.0f };

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
		verts.push_back(
			{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 255, 0, 0, 0 }
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


	MatrixConstantBuffer matConsBuffer = GetCurrentTransform();
	bindables.push_back(std::make_unique<ConstantVS<MatrixConstantBuffer>>(matConsBuffer, 0));
	consBuffer = static_cast<ConstantVS<MatrixConstantBuffer>*>(bindables[bindables.size() - 1].get());


	bindables.push_back(std::make_unique<PixShader>(L"BasicPix.cso"));
	bindables.push_back(std::make_unique<VertShader>(L"BasicVert.cso"));
	ID3DBlob* blob = (static_cast<VertShader*>(bindables[bindables.size() - 1].get()))->GetBlob();

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	bindables.push_back(std::make_unique<InputLayout>(inputLayoutDesc, blob));
}
