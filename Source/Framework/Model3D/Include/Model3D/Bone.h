#pragma once
#include <RhyMath.h>
#include <RhyAssimp.h>

#include <vector>

class Bone
{
	struct Weight 
	{
		unsigned int indexNumber;
		float weight;
	};
public:
	Bone();

	const char* name = nullptr;
	aiMatrix4x4 offset{};
	std::vector<Weight> weights{};
	Bone* parent = nullptr;
};
