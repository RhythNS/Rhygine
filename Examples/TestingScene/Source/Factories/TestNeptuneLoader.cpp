#include <Factories/TestNeptuneLoader.h>
#include <Factories/TestModel.h>
#include <Core/Window.h>
#include <ECS/Bindable/BasicShader.h>
#include <Components/RotateAround.h>
#include <CC/Shaders/ToonShader.h>
#include <ECS/Bindable/Verticies.h>
#include <Model3D/ModelLoader.h>
#include <Model3D/Mesh.h>
#include <RhyBindables.h>
#include <RhyAssimp.h>

TestNeptuneLoader::TestNeptuneLoader(TestLightComponent* tlc) : tlc(tlc)
{
	std::string texNames[4] = { "Assets\\PresentScene\\neptune\\mouth.png", "Assets\\PresentScene\\neptune\\body.png",
		"Assets\\PresentScene\\neptune\\body02.png", "Assets\\PresentScene\\neptune\\eye.png" };

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

	const std::string pFile = "Assets\\PresentScene\\neptune\\neptune.obj";

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
