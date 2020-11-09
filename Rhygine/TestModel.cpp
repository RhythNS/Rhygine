#include "TestModel.h"
#include "RhyBindables.h"
#include "RhyAssimp.h"
#include "Window.h"
#include "BasicShader.h"
#include "RotateAround.h"

void TestModel::AddData(GameObject* toAddTo)
{
	Transform* transform = AddTransform(toAddTo);
	Drawer* drawer = AddDrawer(toAddTo);

	transform->localPosition.setValue(-10.0f, 0.0f, 10.0f);

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
			{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, texVec->x, 1 - texVec->y }
		);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		indexes.push_back(face.mIndices[0]);
		indexes.push_back(face.mIndices[1]);
		indexes.push_back(face.mIndices[2]);
	}

	drawer->AddBindable(std::make_unique<VertBuffer<Vertex>>(verts, 0));

	drawer->AddBindable(std::make_unique<IndexBufferUS>(indexes, 0));

	drawer->AddBindable(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	drawer->AddBindable(std::make_unique<Texture>("TestModels\\spot_texture.png", 0));
	drawer->AddBindable(std::make_unique<Sampler>(0));

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "texCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	drawer->AddBindable(std::make_unique<BasicShader>(L"TexPix.hlsl", L"TexVert.hlsl", &inputLayoutDesc));

	transform->GetGameObject()->AddComponent<RotateAround>()->rotationSpeed = RhyM::Vec3(1.0, 0.3f, 0.6f);
}
