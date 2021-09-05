#include <Factories/TestModelLoader.h>
#include <Factories/TestModel.h>
#include <Core/Window.h>
#include <ECS/Bindable/BasicShader.h>
#include <Components/RotateAround.h>
#include <CC/Shaders/ToonShader.h>
#include <ECS/Bindable/Verticies.h>
#include <Model3D/ModelLoader.h>
#include <Model3D/Mesh.h>
#include <Components/RinModelShower.h>
#include <RhyBindables.h>
#include <RhyAssimp.h>

TestModelLoader::TestModelLoader(TestLightComponent* tlc) : tlc(tlc)
{
}

void TestModelLoader::AddData(GameObject* toAddTo)
{
	Transform* transform = AddTransform(toAddTo);

	transform->localScale.Set(1.0f, 1.0f, 1.0f);
	transform->localPosition.Set(-5.0f, 0.0f, 2.0f);
	transform->localRotation = RhyM::Quat::FromEuler(0, 0, 180);

	const std::string rootFile = "Assets\\PresentScene\\Rin\\";
	const std::string pFile = rootFile + "\\Rin.fbx";

	ModelLoader* loader = ModelLoader::GetInstance();

	loader->LoadScene(pFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	std::vector<Drawer*> drawers = std::vector<Drawer*>(loader->GetMeshCount(), nullptr);
	for (unsigned int i = 0; i < loader->GetMeshCount(); i++)
	{
		std::shared_ptr<Mesh> mesh = loader->LoadMesh(i);

		std::vector<VertexPosNormalUV> verts;
		std::vector<unsigned int> indexes = mesh->indicies;

		verts.reserve(mesh->verticies.size());
		indexes.reserve(mesh->indicies.size());

		Drawer* drawer = AddDrawer(toAddTo);
		drawers[i] = drawer;

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

		aiMaterial* mat = loader->GetMaterial(mesh->materialIndex);
		aiString path;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		std::string acctualPath = rootFile + path.C_Str();

		drawer->AddBindable(std::make_unique<Texture>(acctualPath.c_str(), 0));
		drawer->AddBindable(std::make_unique<Sampler>(0));

		ToonShader* toon = static_cast<ToonShader*>(drawer->AddBindable(std::make_unique<ToonShader>()));
		toon->light = tlc;
	}

	loader->UnLoadScene();
	transform->GetGameObject()->AddComponent<RinModelShower>()->Set(drawers);
	//transform->GetGameObject()->AddComponent<RotateAround>()->rotationSpeed = RhyM::Vec3(0.0f, 1.0f, 0.0f);
}
