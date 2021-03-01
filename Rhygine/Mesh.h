#pragma once
#include "RhyMath.h"
#include "RhyColor.h"
#include "Bone.h"

#include <vector>

class Mesh
{
public:
	Mesh(unsigned int numVerticies, unsigned int numNormals, unsigned int numTangents, unsigned int numBiTangents,
		unsigned int numColorChannels, unsigned int numUVChannels, unsigned int numBones, const char* name);

	bool HasIndicies();
	bool HasNormals();
	bool HasTangents();
	bool HasBiTangents();
	unsigned int GetColorChannelCount();
	unsigned int GetUVChannelCount();
	unsigned int GetBoneCount();

	std::vector<unsigned int> indicies{};
	std::vector<RhyM::Vec3> verticies{};
	std::vector<RhyM::Vec3> normals{};
	std::vector<RhyM::Vec3> tangents{};
	std::vector<RhyM::Vec3> biTangents{};
	std::vector<std::vector<RhyC::color>> colorChannels{};
	std::vector<std::vector<RhyM::Vec3>> uvChannels{};
	std::vector<Bone> bones{};
	Bone* rootBone = nullptr;
	const char* name{};
};
