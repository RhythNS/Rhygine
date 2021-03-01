#include "TestModelLoader.h"
#include "TestModel.h"
#include "RhyBindables.h"
#include "RhyAssimp.h"
#include "Window.h"
#include "BasicShader.h"
#include "RotateAround.h"
#include "ToonShader.h"
#include "Verticies.h"
#include "ModelLoader.h"
#include "Mesh.h"

TestModelLoader::TestModelLoader(TestLightComponent* tlc) : tlc(tlc)
{
	std::string texNames[25] = {
		"PresentScene\\Rin\\Textures\\body_texture.png",
		"PresentScene\\Rin\\Textures\\body_texture_alpha.png",
		"PresentScene\\Rin\\Textures\\Bottoms_01.png",
		"PresentScene\\Rin\\Textures\\F00_000_00_EyeExtra_01.png",
		"PresentScene\\Rin\\Textures\\F00_000_00_Face_00_nml.png",
		"PresentScene\\Rin\\Textures\\F00_000_00_Face_00_out.png",
		"PresentScene\\Rin\\Textures\\F00_000_EyeIris_00.png",
		"PresentScene\\Rin\\Textures\\F00_000_EyeWhite_00.png",
		"PresentScene\\Rin\\Textures\\F00_000_Face_00.png",
		"PresentScene\\Rin\\Textures\\F00_000_FaceBrow_00.png",
		"PresentScene\\Rin\\Textures\\F00_000_FaceEyeline_00.png",
		"PresentScene\\Rin\\Textures\\F00_000_FaceMouth_00.png",
		"PresentScene\\Rin\\Textures\\F00_000_Hair_00.png",
		"PresentScene\\Rin\\Textures\\F00_000_Hair_00_nml.png",
		"PresentScene\\Rin\\Textures\\F00_000_HairBack_00.png",
		"PresentScene\\Rin\\Textures\\F00_000_HairBack_00_nml.png" ,
		"PresentScene\\Rin\\Textures\\F00_001_01_Body_00_nml.png" ,
		"PresentScene\\Rin\\Textures\\Matcap_Rim.png" ,
		"PresentScene\\Rin\\Textures\\Matcap_RimHair.png" ,
		"PresentScene\\Rin\\Textures\\necker1.png" ,
		"PresentScene\\Rin\\Textures\\Ribbon1.png" ,
		"PresentScene\\Rin\\Textures\\Shader_NoneBlack.png" ,
		"PresentScene\\Rin\\Textures\\Shader_NoneBlack_0.png" ,
		"PresentScene\\Rin\\Textures\\Shader_NoneNormal.png" ,
		"PresentScene\\Rin\\Textures\\Shirt.png"
	};

	for (int i = 0; i < 25; i++)
	{
		texs.push_back(std::make_unique<Texture>(texNames[i].c_str(), 0));
	}
}

void TestModelLoader::AddData(GameObject* toAddTo)
{
	Transform* transform = AddTransform(toAddTo);

	transform->localScale.Set(1.0f, 1.0f, 1.0f);
	transform->localPosition.Set(-5.0f, 0.0f, 2.0f);
	transform->localRotation = RhyM::Quat::Identity();

	const std::string pFile = "PresentScene/Rin/Rin.fbx";
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);

	ModelLoader* loader = ModelLoader::GetInstance();

	loader->LoadScene(pFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	for (unsigned int i = 0; i < loader->GetMeshCount(); i++)
	{
		std::shared_ptr<Mesh> mesh = loader->LoadMesh(i);

		std::vector<VertexPosNormalUV> verts;
		std::vector<unsigned int> indexes = mesh->indicies;

		verts.reserve(mesh->verticies.size());
		indexes.reserve(mesh->indicies.size());

		Drawer* drawer = AddDrawer(toAddTo);

		for (unsigned int i = 0; i < mesh->verticies.size(); i++)
		{
			verts.push_back(
				{
				mesh->verticies[i].x, mesh->verticies[i].y, mesh->verticies[i].z, // pos
				mesh->normals[i].x, mesh->normals[i].y, mesh->normals[i].z, // normal
				mesh->uvChannels[0][i].x, 1 - mesh->uvChannels[0][i].y// uv 
				}
			);
		}

		drawer->AddBindable(std::make_unique<VertBuffer<VertexPosNormalUV>>(verts, 0));

		drawer->AddBindable(std::make_unique<IndexBufferUI>(indexes, 0));

		drawer->AddBindable(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		drawer->AddBindable(std::make_unique<Texture>(texs[i].get(), 0));
		drawer->AddBindable(std::make_unique<Sampler>(0));

		ToonShader* toon = static_cast<ToonShader*>(drawer->AddBindable(std::make_unique<ToonShader>()));
		toon->light = tlc;
	}

	loader->UnLoadScene();
	transform->GetGameObject()->AddComponent<RotateAround>()->rotationSpeed = RhyM::Vec3(0.0f, 1.0f, 0.0f);
}
