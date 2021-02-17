#include "TestNeptuneLoader.h"
#include "TestModel.h"
#include "RhyBindables.h"
#include "RhyAssimp.h"
#include "Window.h"
#include "BasicShader.h"
#include "RotateAround.h"
#include "ToonShader.h"
#include "Verticies.h"

TestNeptuneLoader::TestNeptuneLoader(TestLightComponent* tlc) : tlc(tlc)
{
	std::string texNames[4] = { "PresentScene\\neptune\\mouth.png", "PresentScene\\neptune\\body.png",
		"PresentScene\\neptune\\body02.png", "PresentScene\\neptune\\eye.png" };

	for (int i = 0; i < 4; i++)
	{
		texs[i] = std::make_unique<Texture>(texNames[i].c_str(), 0);
	}
}

void TestNeptuneLoader::AddData(GameObject* toAddTo)
{
	Transform* transform = AddTransform(toAddTo);

	transform->localScale.Set(0.02f, 0.02f, 0.02f);
	transform->localPosition.Set(-5.0f, 0.0f, -2.0f);
	transform->localRotation = RhyM::Quat::Identity();

	const std::string pFile = "PresentScene\\neptune\\neptune.obj";
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices);

	for (unsigned int i = 0; i < 4; i++)
	{
		std::vector<VertexPosNormalUV> verts;
		std::vector<unsigned short> indexes;

		Drawer* drawer = AddDrawer(toAddTo);

		const aiMesh* mesh = scene->mMeshes[i];

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			const aiVector3D* texVec = &mesh->mTextureCoords[0][i];
			verts.push_back( { 
				mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, // pos
				mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, // normal
				texVec->x, 1 - texVec->y // uv 
				}
			);

		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];
			indexes.push_back(face.mIndices[0]);
			indexes.push_back(face.mIndices[1]);
			indexes.push_back(face.mIndices[2]);
		}

		drawer->AddBindable(std::make_unique<VertBuffer<VertexPosNormalUV>>(verts, 0));

		drawer->AddBindable(std::make_unique<IndexBufferUS>(indexes, 0));

		drawer->AddBindable(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		drawer->AddBindable(std::make_unique<Texture>(texs[i].get(), 0));
		drawer->AddBindable(std::make_unique<Sampler>(0));

		ToonShader* toon = static_cast<ToonShader*>(drawer->AddBindable(std::make_unique<ToonShader>()));
		toon->light = tlc;
	}
	
	transform->GetGameObject()->AddComponent<RotateAround>()->rotationSpeed = RhyM::Vec3(0.0f, 1.0f, 0.0f);
}
