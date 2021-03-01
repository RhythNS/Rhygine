#include "Mesh.h"

Mesh::Mesh(unsigned int numVerticies, unsigned int numNormals, unsigned int numTangents, unsigned int numBiTangents, unsigned int numColorChannels, unsigned int numUVChannels, unsigned int numBones, const char* name)
	: verticies(numVerticies, { 0.0f,0.0f,0.0f }), normals(numNormals, { 0.0f,0.0f,0.0f }), tangents(numTangents, { 0.0f,0.0f,0.0f }),
	biTangents(numBiTangents, { 0.0f,0.0f,0.0f }), colorChannels(numColorChannels), uvChannels(numUVChannels), bones(numBones, Bone()),
	name(name)
{
}

bool Mesh::HasIndicies()
{
	return indicies.size() > 0;
}

bool Mesh::HasNormals()
{
	return normals.size() > 0;
}

bool Mesh::HasTangents()
{
	return tangents.size() > 0;
}

bool Mesh::HasBiTangents()
{
	return biTangents.size() > 0;
}

unsigned int Mesh::GetColorChannelCount()
{
	return static_cast<unsigned int>(colorChannels.size());
}

unsigned int Mesh::GetUVChannelCount()
{
	return static_cast<unsigned int>(uvChannels.size());
}

unsigned int Mesh::GetBoneCount()
{
	return static_cast<unsigned int>(bones.size());
}
