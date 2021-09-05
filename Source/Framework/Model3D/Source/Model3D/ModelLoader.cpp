#include <Model3D/ModelLoader.h>
#include <Model3d/Mesh.h>

#include <2D/TextureGenerator.h>

#include <cassert>

ModelLoader::ModelLoader()
{
	instance = this;
}

ModelLoader::~ModelLoader()
{
	instance = nullptr;
}

bool ModelLoader::UnLoadScene()
{
	assert(currentScene);

	importer.FreeScene();
	currentScene = nullptr;
	return true;
}

bool ModelLoader::LoadScene(std::string filePath)
{
	return LoadScene(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
}

bool ModelLoader::LoadScene(std::string filePath, unsigned int flags)
{
	currentScene = importer.ReadFile(filePath, flags);

	return currentScene;
}

const char* ModelLoader::GetErrorString()
{
	return importer.GetErrorString();
}

unsigned int ModelLoader::GetMeshCount()
{
	assert(currentScene);

	return currentScene->mNumMeshes;
}

unsigned int ModelLoader::GetAnimationCount()
{
	assert(currentScene);

	return currentScene->mNumAnimations;
}

unsigned int ModelLoader::GetTextureCount()
{
	assert(currentScene);

	return currentScene->mNumTextures;
}

const char* ModelLoader::GetTextureNameAt(unsigned int number)
{
	assert(currentScene && number < currentScene->mNumTextures);

	return currentScene->mTextures[number]->mFilename.C_Str();
}

unsigned int ModelLoader::GatMaterialCount()
{
	assert(currentScene);
	currentScene->mMaterials;
	return currentScene->mNumMaterials;
}

aiMaterial* ModelLoader::GetMaterial(unsigned int number)
{
	assert(currentScene && number < currentScene->mNumMaterials);

	return currentScene->mMaterials[number];
}

std::unique_ptr<Texture> ModelLoader::LoadTexture(unsigned int number, int slot)
{
	assert(currentScene && number < currentScene->mNumTextures);

	// Remove and uncomment line when texture can accept an aitexture again.
	TextureGenerator texGen = TextureGenerator(currentScene->mTextures[number]->mWidth, currentScene->mTextures[number]->mHeight);
	texGen.brush = RhyC::black;
	texGen.Fill(0, 0);
	return std::make_unique<Texture>(&texGen, slot);
	
	//return std::make_unique<Texture>(currentScene->mTextures[number], slot);
}

std::shared_ptr<Mesh> ModelLoader::LoadMesh(unsigned int number)
{
	assert(currentScene && number < currentScene->mNumMeshes);

	const aiMesh* aiMesh = currentScene->mMeshes[number];

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
		aiMesh->mNumVertices,
		aiMesh->HasNormals() ? aiMesh->mNumVertices : 0,
		aiMesh->HasTangentsAndBitangents() ? aiMesh->mNumVertices : 0,
		aiMesh->HasTangentsAndBitangents() ? aiMesh->mNumVertices : 0,
		aiMesh->GetNumColorChannels(),
		aiMesh->GetNumUVChannels(),
		aiMesh->HasBones() ? aiMesh->mNumBones : 0,
		aiMesh->mName.C_Str(),
		aiMesh->mMaterialIndex
		);

	for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
	{
		mesh->verticies[i] = { aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z };
	}

	if (aiMesh->HasFaces())
	{
		for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
		{
			for (unsigned int j = 0; j < aiMesh->mFaces[i].mNumIndices; j++)
			{
				mesh->indicies.push_back(aiMesh->mFaces[i].mIndices[j]);
			}
		}
	}

	if (aiMesh->HasNormals())
	{
		for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
		{
			mesh->normals[i] = { aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z };
		}
	}

	if (aiMesh->HasTangentsAndBitangents())
	{
		for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
		{
			mesh->tangents[i] = { aiMesh->mTangents[i].x, aiMesh->mTangents[i].y, aiMesh->mTangents[i].z };
			mesh->biTangents[i] = { aiMesh->mBitangents[i].x, aiMesh->mBitangents[i].y, aiMesh->mBitangents[i].z };
		}
	}

	for (unsigned int i = 0; i < aiMesh->GetNumColorChannels(); i++)
	{
		if (aiMesh->HasVertexColors(i))
		{
			mesh->colorChannels[i].reserve(aiMesh->mNumVertices);
			for (unsigned int j = 0; j < aiMesh->mNumVertices; j++)
			{
				mesh->colorChannels[i].push_back(RhyC::FromFloat(aiMesh->mColors[i][j].r, aiMesh->mColors[i][j].g, aiMesh->mColors[i][j].b, aiMesh->mColors[i][j].a));
			}
		}
	}

	for (unsigned int i = 0; i < aiMesh->GetNumUVChannels(); i++)
	{
		mesh->uvChannels[i].reserve(aiMesh->mNumVertices);
		for (unsigned int j = 0; j < aiMesh->mNumVertices; j++)
		{
			mesh->uvChannels[i].push_back(RhyM::Vec3(aiMesh->mTextureCoords[i][j].x, aiMesh->mTextureCoords[i][j].y, aiMesh->mTextureCoords[i][j].z));
		}
	}

	if (aiMesh->HasBones())
	{
		aiNode* rootBone = GetRootBone(aiMesh, currentScene->mRootNode);

		std::unordered_map<std::string, Bone*> bones;
		for (unsigned int i = 0; i < aiMesh->mNumBones; i++)
		{
			mesh->bones[i].name = aiMesh->mBones[i]->mName.C_Str();
			mesh->bones[i].offset = aiMesh->mBones[i]->mOffsetMatrix;

			mesh->bones[i].weights.reserve(aiMesh->mBones[i]->mNumWeights);
			for (unsigned int j = 0; j < aiMesh->mBones[i]->mNumWeights; j++)
			{
				mesh->bones[i].weights.push_back({ aiMesh->mBones[i]->mWeights[j].mVertexId, aiMesh->mBones[i]->mWeights[j].mWeight });
			}
			auto a = &mesh->bones[i];
			bones[mesh->bones[i].name] = &mesh->bones[i];
		}

		for (unsigned int i = 0; i < rootBone->mNumChildren; i++)
		{
			AssignBone(&bones, rootBone->mChildren[i]);
		}
	}

	return mesh;
}

ModelLoader* ModelLoader::GetInstance()
{
	return instance;
}

aiNode* ModelLoader::GetRootBone(const aiMesh* aiMesh, aiNode* currentNode)
{
	for (unsigned int i = 0; i < aiMesh->mNumBones; i++)
	{
		if (aiMesh->mBones[i]->mName == currentNode->mName)
			return currentNode;
	}

	for (unsigned int i = 0; i < currentNode->mNumChildren; i++)
	{
		aiNode* toRet = GetRootBone(aiMesh, currentNode->mChildren[i]);
		if (toRet != nullptr)
			return toRet;
	}

	return nullptr;
}

void ModelLoader::AssignBone(std::unordered_map<std::string, Bone*>* bones, aiNode* currentNode)
{
	Bone* currentBone = (*bones)[currentNode->mName.C_Str()];
	if (currentBone == nullptr)
		return;

	currentBone->parent = (*bones)[currentNode->mParent->mName.C_Str()];

	for (unsigned int i = 0; i < currentNode->mNumChildren; i++)
	{
		AssignBone(bones, currentNode->mChildren[i]);
	}
}

ModelLoader* ModelLoader::instance = nullptr;
