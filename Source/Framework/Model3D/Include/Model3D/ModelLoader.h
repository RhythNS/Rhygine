#pragma once
#include <RhyAssimp.h>
#include <ECS/Bindable/Texture.h>

#include <memory>
#include <unordered_map>

class Mesh;
class Bone;

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	bool UnLoadScene();
	bool LoadScene(std::string filePath);
	bool LoadScene(std::string filePath, unsigned int flags);
	const char* GetErrorString();

	unsigned int GetMeshCount();
	unsigned int GetAnimationCount();
	unsigned int GetTextureCount();
	const char* GetTextureNameAt(unsigned int number);
	unsigned int GatMaterialCount();
	aiMaterial* GetMaterial(unsigned int number);

	std::unique_ptr<Texture> LoadTexture(unsigned int number, int slot);
	std::shared_ptr<Mesh> LoadMesh(unsigned int number);

	static ModelLoader* GetInstance();

private:
	static ModelLoader* instance;
	Assimp::Importer importer;
	const aiScene* currentScene = nullptr;

	aiNode* GetRootBone(const aiMesh* aiMesh, aiNode* currentNode);
	void AssignBone(std::unordered_map<std::string, Bone*>* bones, aiNode* currentNode);
};
